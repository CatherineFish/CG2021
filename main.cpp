#include "common.h"

#include "Image.h"

#include "Player.h"

#include <iostream>

//#define GLFW_DLL

struct InputState {
    bool keys[1024] {}; //массив состояний кнопок - нажата/не нажата
    GLfloat lastX = 400, lastY = 300; //исходное положение мыши
    bool firstMouse = true;
    bool captureMouse = true; // Мышка захвачена нашим приложением или нет?
    bool capturedMouseJustNow = false;
}
static Input;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

void OnKeyboardPressed(GLFWwindow * window, int key, int scancode, int action, int mode) {
    switch (key) {
    case GLFW_KEY_ESCAPE:
        if (action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
        break;
    case GLFW_KEY_1:
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        break;
    case GLFW_KEY_2:
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        break;
    default:
        if (action == GLFW_PRESS && key < sizeof(Input.keys))
            Input.keys[key] = true;
        else if (action == GLFW_RELEASE && key < sizeof(Input.keys))
            Input.keys[key] = false;
    }
}

bool processPlayerMovement(Player & player, Level & cur_level, Image & screenBuffer) {
    //std::cout << "HERE\n";
    if (player.is_finish || player.is_win)
    {
        if (Input.keys[GLFW_KEY_ENTER])
        {
            return true;
        }
    }

    if (Input.keys[GLFW_KEY_E])
        player.ProcessInput(MovementDir::SPACE, cur_level, screenBuffer);
    if (Input.keys[GLFW_KEY_W])
        player.ProcessInput(MovementDir::UP, cur_level, screenBuffer);
    else if (Input.keys[GLFW_KEY_S])
        player.ProcessInput(MovementDir::DOWN, cur_level, screenBuffer);
    if (Input.keys[GLFW_KEY_A])
        player.ProcessInput(MovementDir::LEFT, cur_level, screenBuffer);
    else if (Input.keys[GLFW_KEY_D])
        player.ProcessInput(MovementDir::RIGHT, cur_level, screenBuffer);
    return false;

}

void OnMouseButtonClicked(GLFWwindow * window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
        Input.captureMouse = !Input.captureMouse;

    if (Input.captureMouse) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        Input.capturedMouseJustNow = true;
    } else
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

}

void OnMouseMove(GLFWwindow * window, double xpos, double ypos) {
    if (Input.firstMouse) {
        Input.lastX = float(xpos);
        Input.lastY = float(ypos);
        Input.firstMouse = false;
    }

    GLfloat xoffset = float(xpos) - Input.lastX;
    GLfloat yoffset = Input.lastY - float(ypos);

    Input.lastX = float(xpos);
    Input.lastY = float(ypos);
}

void OnMouseScroll(GLFWwindow * window, double xoffset, double yoffset) {
    // ...
}

int initGL() {
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return -1;
    }

    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    std::cout << "Controls: " << std::endl;
    std::cout << "press right mouse button to capture/release mouse cursor  " << std::endl;
    std::cout << "W, A, S, D - movement  " << std::endl;
    std::cout << "press ENTER to restart" << std::endl;
    std::cout << "press E to use the key to open the door" << std::endl;
    std::cout << "press ESC to exit" << std::endl;

    return 0;
}

int main(int argc, char ** argv) {
    if (!glfwInit())
        return -1;

    //	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow * window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "task1 base project", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    Image screenBuffer(WINDOW_WIDTH, WINDOW_HEIGHT, 4);

    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, OnKeyboardPressed);
    glfwSetCursorPosCallback(window, OnMouseMove);
    glfwSetMouseButtonCallback(window, OnMouseButtonClicked);
    glfwSetScrollCallback(window, OnMouseScroll);

    if (initGL() != 0)
        return -1;

    //Reset any OpenGL errors which could be present for some reason
    GLenum gl_error = glGetError();
    while (gl_error != GL_NO_ERROR)
        gl_error = glGetError();

    Level cur_lev("./resources/room_templ_", "./resources/comm_lab.txt");

    Point starting_pos {
        .x = cur_lev.x_hero[0][cur_lev.lev_num] * tileSize, .y = WINDOW_HEIGHT - cur_lev.y_hero[0][cur_lev.lev_num] * tileSize
    };
    Player player {
        starting_pos
    };

    //game loop
    while (!glfwWindowShouldClose(window)) {

        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        GL_CHECK_ERRORS;
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        GL_CHECK_ERRORS;
        cur_lev.Level_Draw(screenBuffer, player.is_finish, player.is_win);
        cur_lev.Key_Draw(screenBuffer, player.key_num);
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();
        if (processPlayerMovement(player, cur_lev, screenBuffer))
        {
            cur_lev = std::move(Level("./resources/room_templ_", "./resources/comm_lab.txt"));
            starting_pos = std::move(Point {.x = cur_lev.x_hero[0][cur_lev.lev_num] * tileSize, .y = WINDOW_HEIGHT - cur_lev.y_hero[0][cur_lev.lev_num] * tileSize});
            player = std::move(Player{starting_pos});
        }
        player.Draw(screenBuffer);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        GL_CHECK_ERRORS;
        glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data());
        GL_CHECK_ERRORS;
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
