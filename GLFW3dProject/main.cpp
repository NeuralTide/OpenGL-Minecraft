#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <iostream>
#include "SimplexNoise.h"

#include "shader_s.h"
#include "ChunkLoader.h"
#include "Chunk.h"
#include <vector>
#include <list>
#include "Player.h"
#include "ThreadPool.h"
#include "Feature.h";

extern "C"
{
    __declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

//terrain gen

// settings
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 1200;
Player player;
Feature fobjects;


//time
float deltaTime = 0.0f;
float lastFrame = 0.0f;


unsigned int tex;

int main()
{
    player = Player(glm::vec3(0, 120, 0));
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
    GLFWwindow* parentWindow = glfwCreateWindow(1, 1, "Main Window", NULL, NULL);

    // glfw window creation
    // --------------------
    glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Minecraft Clone", NULL, parentWindow);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);



    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


    int width, height, nrChannels;
    unsigned char* data = stbi_load("resources/mccatlas.jpg", &width, &height, &nrChannels, 0);
    if (data)
    {

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

    }
    else
    {

        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


    glActiveTexture(GL_TEXTURE0);
   

    std::vector<Chunk> chunks;
    ChunkLoader chunkLoader = ChunkLoader();

    ThreadPool chunk_threads = ThreadPool(2);
    fobjects = Feature();
  
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        //calculate time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------

        if (player.getCameraPostion().y > 70) {
            glClearColor(.79f, .91f, .98f, 0.7f);
        }
        else {
            glClearColor(0, 0, 0, 0);
        }
       
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_MULTISAMPLE);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    




        glm::mat4 view = player.getCameraView();
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(90.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 500.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        glm::mat4 model = glm::mat4(1.0f);
        //make sure all chunks should be loaded, if not, unload, genereate new chunks when needed and finally draw all active chunks.
        chunkLoader.manageChunks(model, projection, view, player.getPlayerPosition(), &chunk_threads, &fobjects);
  
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    chunkLoader.joinAllThreads();
    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    chunkLoader.deleteChunkBuffers();
    


    
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    player.manageMovement(window, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    player.updateCameraRotation(xpos, ypos);
}