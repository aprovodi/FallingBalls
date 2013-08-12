#include <list>
#include <algorithm>
#include <functional>
#include <sstream>
#include <time.h>

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "MShape.hpp"

// Refresh period of generating new ball in milliseconds
const int refreshMillis = 500;

float MShape::initial_speed = 5.0f;
const float MShape::max_initial_speed = 10.0f;
const float MShape::min_initial_speed = 1.0f;

/* LIST is used here as we delete often at different locations of the container;
	pointers are used here as we have polymorphic abstract class MShape */
typedef std::list<MShape::MShapePtr> MShapesContainer;
static MShapesContainer balls;

static int playerResult = 0;

/* Timer function to generate new ball every *ms and add it to the collection */
void generateNewBall(int v)
{
	float radius = 3*Utils::randRange(3.f, 10.f);
	float xpos = Utils::randRange(radius, Utils::WINDOW_WIDTH - radius);

	MShape::MShapePtr b = std::make_shared<FallingBall>(radius, xpos, 0.0f);
	balls.push_back(b);

	glutTimerFunc(refreshMillis, generateNewBall, 1);
}

/* function to draw string to the window */
void drawText(const std::string& str, int x, int y)
{
	glColor3f(1.0, 1.0, 1.0);
	glRasterPos2f(x, y);
	for (std::size_t i = 0; i < str.length(); i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
}

/* main rendering functio to draw our scene */
void Render()
{
	/* update balls positions */
	std::for_each(balls.begin(), balls.end(), std::mem_fn(&MShape::update));

	/* erase balls which exceed boundary */
	balls.erase(std::remove_if(balls.begin(), balls.end(), std::mem_fn(&MShape::isBoundaryReached)), balls.end());

	/* draw each ball from the container */
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	std::for_each(balls.begin(), balls.end(), std::mem_fn(&MShape::draw));

	std::ostringstream os;
	os << "Score: " << playerResult;
	drawText(os.str().c_str(), Utils::WINDOW_WIDTH - 150, Utils::WINDOW_HEIGHT - 20);
	os.seekp(0);

	glutSwapBuffers();  // Swap front and back buffers (of double buffered mode)
}

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		std::pair<int,int> mouse_click = std::pair<int,int>(x,y);
		/* iterate over list, find and erase balls were clicked */
		MShapesContainer::iterator i = balls.begin();
		while (i != balls.end())
		{
			std::shared_ptr<FallingBall> fb = std::dynamic_pointer_cast<FallingBall>(*i);
			if (fb->isPointInside(mouse_click))
			{
				playerResult += fb->getCost();
				i = balls.erase(i);
			}
			else ++i;
		}
	}
}

/* OpenGL Setting */
void Setting(void) 
{
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glViewport(0, 0, Utils::WINDOW_WIDTH, Utils::WINDOW_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, Utils::WINDOW_WIDTH, Utils::WINDOW_HEIGHT, 0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/* OnWindowResize, currently no resizing */
void reshape (GLsizei width, GLsizei height)
{
	if( width != Utils::WINDOW_WIDTH || height != Utils::WINDOW_HEIGHT )
	{
		glutReshapeWindow( Utils::WINDOW_WIDTH, Utils::WINDOW_HEIGHT);
	}
}

/* Called back when there is no other event to be handled */
void idle() {
	glutPostRedisplay();   // Post a re-paint request to activate display()
}

/* keyboard messages */
void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
		exit(0);
		break;
	}
}

/* keyboard message */
void inputKey(int key, int x, int y)
{
	float scale = 1.3f;
	switch (key) {
	case GLUT_KEY_UP : 
		// speed up falling balls
		MShape::sqInitialSpeed(scale);
		std::for_each(balls.begin(), balls.end(), std::bind(std::mem_fn(&MShape::increaseSpeed), std::placeholders::_1, scale));
		break ;

	case GLUT_KEY_DOWN : 
		// slow down falling balls
		MShape::hfInitialSpeed(scale);
		std::for_each(balls.begin(), balls.end(), std::bind(std::mem_fn(&MShape::decreaseSpeed), std::placeholders::_1, scale));
		break;
	}
}

int main(int argc, char** argv)
{
	// Seed random number generator
	srand(time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(Utils::WINDOW_WIDTH, Utils::WINDOW_HEIGHT); 
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Osama Hosam's simple OpenGL game");

	Setting ();

	glutDisplayFunc(Render);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutTimerFunc(refreshMillis, generateNewBall, 1);

	glutKeyboardFunc(keyboard);
	glutSpecialFunc(inputKey);

	glutMouseFunc(mouse);
	glutMainLoop();

	return 0;
}
