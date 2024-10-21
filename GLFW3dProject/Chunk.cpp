#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>
#include <vector>
#include <thread>
#include <functional>

#include "shader_s.h"
#include "Chunk.h"
#include "LayeredNoise.h"
#include "glm/glm.hpp"
#include "TextureManager.h"



using std::vector;

    const int WIDTH = 32;
    const int DEPTH = WIDTH;
    const int HEIGHT = 32;
   

    const int NEXT_LEFT = HEIGHT;
    const int NEXT_RIGHT = -HEIGHT;
    const int NEXT_FRONT = HEIGHT * WIDTH;
    const int NEXT_BACK = -(HEIGHT * WIDTH);

    int posX;
    int posZ;
    int posY;
    bool baked;
    bool chunkBuild = false;
    bool firstBake = false;
    size_t bSize = 32768;
    char blocks[];
   
    char* hashedBlocks;
    int vCount;
    int cCounter;
    int v_size;
    float vertices[];
    bool bGen;

   

    unsigned int VBO, VAO;

    vector<float> chunk_verts = {};

    typedef struct {

        TextureManager::blocks c_top;
        TextureManager::blocks c_bottom;
        TextureManager::blocks c_left;
        TextureManager::blocks c_right;
        TextureManager::blocks c_front;
        TextureManager::blocks c_back;

    } b_look;

    std::vector<b_look> block_table = {};
    

    Shader s1;
    unsigned int texture;

 

    /*
        Constructor for a 16x16x16 Chunk

    */
    Chunk::Chunk() {
     
        this->vCount = 0;
        this->baked = false;
        this->bGen = false;
    }

    void Chunk::start(int x, int y, int z, ThreadPool *t, Feature *f){
        this->blocks = (char*)malloc(bSize * sizeof(char));
        

        setChunkPos(x, y, z);
        populateBlockTable();
        auto self = shared_from_this();

        
        t->QueueJob([this, f] { firstChunkGen(f); });
      
       
    }



    void Chunk::deleteChunkData() {

        free(blocks);
     }

    void Chunk::populateBlockTable() {

        // id = 0
        this->block_table.push_back(b_look());
        this->block_table.at(this->block_table.size() - 1).c_top = TextureManager::DIRT;
        this->block_table.at(this->block_table.size() - 1).c_bottom = TextureManager::DIRT;
        this->block_table.at(this->block_table.size() - 1).c_left = TextureManager::DIRT;
        this->block_table.at(this->block_table.size() - 1).c_right = TextureManager::DIRT;
        this->block_table.at(this->block_table.size() - 1).c_front = TextureManager::DIRT;
        this->block_table.at(this->block_table.size() - 1).c_back = TextureManager::DIRT;

        // id = 1

        this->block_table.push_back(b_look());
        this->block_table.at(this->block_table.size() - 1).c_top = TextureManager::GRASS_TOP;
        this->block_table.at(this->block_table.size() - 1).c_bottom = TextureManager::DIRT;
        this->block_table.at(this->block_table.size() - 1).c_left = TextureManager::GRASS_SIDE;
        this->block_table.at(this->block_table.size() - 1).c_right = TextureManager::GRASS_SIDE;
        this->block_table.at(this->block_table.size() - 1).c_front = TextureManager::GRASS_SIDE;
        this->block_table.at(this->block_table.size() - 1).c_back = TextureManager::GRASS_SIDE;

        // id = 2

       this->block_table.push_back(b_look());
       this->block_table.at(this->block_table.size() - 1).c_top = TextureManager::STONE;
       this->block_table.at(this->block_table.size() - 1).c_bottom = TextureManager::STONE;
       this->block_table.at(this->block_table.size() - 1).c_left = TextureManager::STONE;
       this->block_table.at(this->block_table.size() - 1).c_right = TextureManager::STONE;
       this->block_table.at(this->block_table.size() - 1).c_front = TextureManager::STONE;
       this->block_table.at(this->block_table.size() - 1).c_back = TextureManager::STONE;

        // id = 3

       this->block_table.push_back(b_look());
       this->block_table.at(this->block_table.size() - 1).c_top = TextureManager::LOG_TOP;
       this->block_table.at(this->block_table.size() - 1).c_bottom = TextureManager::LOG;
       this->block_table.at(this->block_table.size() - 1).c_left = TextureManager::LOG;
       this->block_table.at(this->block_table.size() - 1).c_right = TextureManager::LOG;
       this->block_table.at(this->block_table.size() - 1).c_front = TextureManager::LOG;
       this->block_table.at(this->block_table.size() - 1).c_back = TextureManager::LOG;
                          
        // id = 4

        this->block_table.push_back(b_look());
        this->block_table.at(this->block_table.size() - 1).c_top = TextureManager::LEAF;
        this->block_table.at(this->block_table.size() - 1).c_bottom = TextureManager::LEAF;
        this->block_table.at(this->block_table.size() - 1).c_left = TextureManager::LEAF;
        this->block_table.at(this->block_table.size() - 1).c_right = TextureManager::LEAF;
        this->block_table.at(this->block_table.size() - 1).c_front = TextureManager::LEAF;
        this->block_table.at(this->block_table.size() - 1).c_back = TextureManager::LEAF;

        // id = 5

        this->block_table.push_back(b_look());
        this->block_table.at(this->block_table.size() - 1).c_top = TextureManager::BEDROCK;
        this->block_table.at(this->block_table.size() - 1).c_bottom = TextureManager::BEDROCK;
        this->block_table.at(this->block_table.size() - 1).c_left = TextureManager::BEDROCK;
        this->block_table.at(this->block_table.size() - 1).c_right = TextureManager::BEDROCK;
        this->block_table.at(this->block_table.size() - 1).c_front = TextureManager::BEDROCK;
        this->block_table.at(this->block_table.size() - 1).c_back = TextureManager::BEDROCK;


        this->block_table.push_back(b_look());
        this->block_table.at(this->block_table.size() - 1).c_top =      TextureManager::SNOW;
        this->block_table.at(this->block_table.size() - 1).c_bottom =   TextureManager::SNOW;
        this->block_table.at(this->block_table.size() - 1).c_left =     TextureManager::SNOW;
        this->block_table.at(this->block_table.size() - 1).c_right =    TextureManager::SNOW;
        this->block_table.at(this->block_table.size() - 1).c_front =    TextureManager::SNOW;
        this->block_table.at(this->block_table.size() - 1).c_back =     TextureManager::SNOW;


    }

    bool Chunk::loadTexture() {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);



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


        return true;
    }

   

    void Chunk::firstChunkGen(Feature *f) {
       
        
      
        int position = 0;

        for (int x = 0; x < WIDTH; x++)
        {
            for (int z = 0; z < HEIGHT; z++)
            {

                int noise = LayeredNoise::getHeightNoise(x + this->posX, z + this->posZ);
                for (int y = 0; y < 32; y++) {


                    if (f->getFeatureBlock(x + this->posX, y + this->posY, z + this->posZ) != '!') {
                        this->blocks[position] = f->getFeatureBlock(x + this->posX, y + this->posY, z + this->posZ);
                        position++;
                    }
                    else if (y + this->posY < 2) {
                        this->blocks[position] = 'B';
                        position++;
                    }
                    else if (y + this->posY < 100) {
                        if (LayeredNoise::getCaveNoise(x + this->posX, y + this->posY, z + this->posZ) > 0) {
                            this->blocks[position] = 'S';
                            position++;

                        }
                        else {
                            this->blocks[position] = 'A';
                            position++;
                        }

                    }
                    else if (y + this->posY > 145 && y + this->posY < 100 + noise) {


                        this->blocks[position] = 'O';
                        position++;


                    }
                    else if (y + this->posY > 120 && y + this->posY < 100 + noise) {


                        this->blocks[position] = 'S';
                        position++;



                    }
                    else if (y + this->posY >= 100 && y + this->posY < 100 + noise) {

                        this->blocks[position] = 'D';
                        position++;


                    }
                    else if (this->blocks[position - 1] == 'D') {

                        this->blocks[position] = 'G';
                        position++;

                    }
                    else if (this->blocks[position - 1] == 'S' && y + this->posY > 140) {
                        this->blocks[position] = 'O';
                        position++;

                    }
                    else {
                        this->blocks[position] = 'A';
                        position++;
                    }

                }





            }
           
        }
      
        genChunkMesh();
       

        
    }

    /*
        Helper function which combindes face vertex array and face texture coordinate vector and pushes the vertices to chunk_verts
    */

    void Chunk::pushToVertices(std::vector<float> eArray, std::vector<float> tc, std::vector<float> *c) {

      
      
        int texc = 0;
        int tIndex = 0;
        int index = 0;
        for (int i = 0; i < 48; i++)
        {

            if (texc == 3 || texc == 4) {
                this->chunk_verts.push_back(tc.at(tIndex));
                tIndex++;  
            }
            else {

                this->chunk_verts.push_back(eArray.at(index));
                index++;
            }
                

            texc++;
            if (texc > 7) {
                texc = 0;
            }
           
           
        }

        
    }

    /*
        Main logic for creating the mesh data
    
    */
    void Chunk::genChunkMesh() {
    
        TextureManager tf = TextureManager(5);
        cCounter = 0;
        for (int x = 0; x < WIDTH; x++) {
            for (int z = 0; z < DEPTH; z++) { 
                for (int y = 0; y < HEIGHT; y++) {
                            int noise = LayeredNoise::getHeightNoise(x + this->posX, z + this->posZ);
                   
                           b_look fb;
                           if (this->blocks[cCounter] == 'D')
                               fb = this->block_table.at(0);
                           if (this->blocks[cCounter] == 'G')
                               fb = this->block_table.at(1);
                           if (this->blocks[cCounter] == 'S')
                               fb = this->block_table.at(2);
                           if (this->blocks[cCounter] == 'L')
                               fb = this->block_table.at(3);
                           if (this->blocks[cCounter] == 'F')
                               fb = this->block_table.at(4);
                           if (this->blocks[cCounter] == 'B')
                               fb = this->block_table.at(5);
                           if (this->blocks[cCounter] == 'O')
                               fb = this->block_table.at(6);
                          
                            
                               if ((z == 0 && blocks[cCounter] != 'A') || (blocks[cCounter] != 'A' && (cCounter >= NEXT_LEFT && blocks[cCounter - NEXT_LEFT] == 'A'))) {
                                   //left face
                                   // v1, v2, v3, t1, t2, n1, n2 ,n3
                                   std::vector<float> left_face = { -0.5f + x + posX,    -0.5f + y + posY,     -0.5f + z + posZ,          0.0f, 0.0f, -1.0f,
                                                            0.5f + x + posX,    -0.5f + y + posY,     -0.5f + z + posZ,          0.0f, 0.0f, -1.0f,
                                                            0.5f + x + posX,     0.5f + y + posY,     -0.5f + z + posZ,          0.0f, 0.0f, -1.0f,

                                                            0.5f + x + posX,     0.5f + y + posY,     -0.5f + z + posZ,          0.0f, 0.0f, -1.0f,
                                                           -0.5f + x + posX,    0.5f + y + posY,     -0.5f + z + posZ,          0.0f, 0.0f, -1.0f,
                                                           -0.5f + x + posX,   -0.5f + y + posY,     -0.5f + z + posZ,          0.0f, 0.0f, -1.0f

                                   };

                                   this->pushToVertices(left_face, tf.getFaceCoords(fb.c_left, TextureManager::LEFT), &chunk_verts);
                                   this->vCount += 6;
                               }

                             

                               if ((z == 31 && blocks[cCounter] != 'A') || (blocks[cCounter] != 'A' && cCounter < bSize - HEIGHT && blocks[cCounter + HEIGHT] == 'A')) {
                                   //right face
                                   //z=0
                                   std::vector<float> right_face =  { -0.5f + x + posX,   -0.5f + y + posY,     0.5f + z + posZ,      0.0f, 0.0f, 1.0f,
                                                                     0.5f + x + posX,   -0.5f + y + posY,     0.5f + z + posZ,      0.0f, 0.0f, 1.0f,
                                                                     0.5f + x + posX,    0.5f + y + posY,     0.5f + z + posZ,      0.0f, 0.0f, 1.0f,

                                                                     0.5f + x + posX,    0.5f + y + posY,     0.5f + z + posZ,      0.0f, 0.0f, 1.0f,
                                                                    -0.5f + x + posX,    0.5f + y + posY,     0.5f + z + posZ,      0.0f, 0.0f, 1.0f,
                                                                    -0.5f + x + posX,   -0.5f + y + posY,     0.5f + z + posZ,      0.0f, 0.0f, 1.0f

                                   };

                                   pushToVertices(right_face, tf.getFaceCoords(fb.c_right, TextureManager::RIGHT), &chunk_verts);
                                   this->vCount += 6;
                               }

                               //front
                               // x = 63
                              
                               if ((x == 31 && blocks[cCounter] != 'A') || (blocks[cCounter] != 'A' && cCounter < (bSize - NEXT_FRONT) && blocks[cCounter + NEXT_FRONT] == 'A')) {
                           
                                   
                                       std::vector<float> front_face = { 0.5f + x + posX,    0.5f + y + posY,     0.5f + z + posZ,       1.0f, 0.0f, 0.0f,
                                                             0.5f + x + posX,    0.5f + y + posY,    -0.5f + z + posZ,       1.0f, 0.0f, 0.0f,
                                                             0.5f + x + posX,   -0.5f + y + posY,    -0.5f + z + posZ,       1.0f, 0.0f, 0.0f,
                                                                                         
                                                             0.5f + x + posX,   -0.5f + y + posY,    -0.5f + z + posZ,       1.0f, 0.0f, 0.0f,
                                                             0.5f + x + posX,   -0.5f + y + posY,     0.5f + z + posZ,       1.0f, 0.0f, 0.0f,
                                                             0.5f + x + posX,    0.5f + y + posY,     0.5f + z + posZ,       1.0f, 0.0f, 0.0f

                                   };
                                   pushToVertices(front_face, tf.getFaceCoords(fb.c_front, TextureManager::FRONT), &chunk_verts);
                                   this->vCount += 6;
                               }
                               //back
                               // x = 0
                             
                               if ((x == 0 && blocks[cCounter] != 'A')  || (blocks[cCounter] != 'A' && cCounter > NEXT_FRONT && blocks[cCounter - NEXT_FRONT] == 'A')) {
                                   std::vector<float> back_face = { -0.5f + x + posX,      0.5f + y + posY,     0.5f + z + posZ,       -1.0f, 0.0f, 0.0f,
                                                           -0.5f + x + posX,    0.5f + y + posY,    -0.5f + z + posZ,       -1.0f, 0.0f, 0.0f,
                                                           -0.5f + x + posX,   -0.5f + y + posY,    -0.5f + z + posZ,       -1.0f, 0.0f, 0.0f,
                                                                                        
                                                           -0.5f + x + posX,   -0.5f + y + posY,    -0.5f + z + posZ,       -1.0f, 0.0f, 0.0f,
                                                           -0.5f + x + posX,   -0.5f + y + posY,     0.5f + z + posZ,       -1.0f, 0.0f, 0.0f,
                                                           -0.5f + x + posX,    0.5f + y + posY,     0.5f + z + posZ,       -1.0f, 0.0f, 0.0f
                                                                                        
                                   };
                                   pushToVertices(back_face, tf.getFaceCoords(fb.c_back, TextureManager::BACK), &chunk_verts);
                                   this->vCount += 6;
                               }
                               if ((y == 31 && blocks[cCounter] != 'A') || blocks[cCounter] != 'A' && cCounter < bSize && blocks[cCounter + 1] == 'A') {
                                   //top
                                   std::vector<float> top_face = { -0.5f + x + posX,    0.5f + y + posY,    -0.5f + z + posZ,        0.0f, 1.0f, 0.0f,
                                                         0.5f + x + posX,    0.5f + y + posY,    -0.5f + z + posZ,        0.0f, 1.0f, 0.0f,
                                                         0.5f + x + posX,    0.5f + y + posY,     0.5f + z + posZ,        0.0f, 1.0f, 0.0f,
                                                                                      
                                                         0.5f + x + posX,    0.5f + y + posY,     0.5f + z + posZ,        0.0f, 1.0f, 0.0f,
                                                        -0.5f + x + posX,    0.5f + y + posY,     0.5f + z + posZ,        0.0f, 1.0f, 0.0f,
                                                        -0.5f + x + posX,    0.5f + y + posY,    -0.5f + z + posZ,        0.0f, 1.0f, 0.0f

                                   };
                                   pushToVertices(top_face, tf.getFaceCoords(fb.c_top, TextureManager::TOP), &chunk_verts);
                                   this->vCount += 6;
                               }
                               //bottom
                               if ((y == 0 && blocks[cCounter] != 'A') || blocks[cCounter] != 'A' && cCounter > 0 && blocks[cCounter - 1] == 'A') {
                                   std::vector<float> bottom_face = { -0.5f + x + posX,    -0.5f + y + posY,    -0.5f + z + posZ,       0.0f, -1.0f, 0.0f,
                                                             0.5f + x + posX,   -0.5f + y + posY,    -0.5f + z + posZ,       0.0f, -1.0f, 0.0f,
                                                             0.5f + x + posX,   -0.5f + y + posY,     0.5f + z + posZ,       0.0f, -1.0f, 0.0f,
                                                                                         
                                                             0.5f + x + posX,   -0.5f + y + posY,     0.5f + z + posZ,       0.0f, -1.0f, 0.0f,
                                                            -0.5f + x + posX,   -0.5f + y + posY,     0.5f + z + posZ,       0.0f, -1.0f, 0.0f,
                                                            -0.5f + x + posX,   -0.5f + y + posY,    -0.5f + z + posZ,       0.0f, -1.0f, 0.0f

                                   };
                                   pushToVertices(bottom_face, tf.getFaceCoords(fb.c_bottom, TextureManager::BOTTOM), &chunk_verts);
                                   this->vCount += 6;
                               }
                           

                           
                          
                         
                       
                        cCounter++;
                }
            }
        
        }

       this->baked = true;
      
    }

    void Chunk::generateBufferData() {
        if (this->chunk_verts.size() > 0) {

            this->vertices = &(this->chunk_verts[0]);

            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);

            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, this->vCount * 32, this->vertices, GL_STATIC_DRAW);


            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
            glEnableVertexAttribArray(2);

        }
       
    }


    /*
        Attempts to decrease mesh complexity by checking to see if
        blocks are not visible
    */
    bool Chunk::bakeChunk() {

        return false;
    }

    bool Chunk::isBaked() {
        return baked;
    }

    /*
        
   
    bool updateChunk() {

        return false;
    }
     */


    bool Chunk::drawChunk(glm::mat4 model, glm::mat4 projection, glm::mat4 view, glm::vec3 cameraPos) {

        if (this->chunk_verts.size() > 0) {
            if (this->baked) {

                if (!this->bGen) {
                    // loadTexture();
                    this->s1 = Shader("chunk_texture.vs", "chunk_texture.fs");

                    generateBufferData();

                    this->bGen = true;
                }


                this->s1.use();
                this->s1.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
                this->s1.setVec3("lightPos", glm::vec3(40.0f, 200.0f, 40.0f));
                this->s1.setVec3("viewPos", cameraPos);

                this->s1.setMat4("model", model);

                this->s1.setMat4("projection", projection);
                this->s1.setMat4("view", view);

                glBindVertexArray(this->VAO);
                glDrawArrays(GL_TRIANGLES, 0, this->vCount);

            }
        }
       

        return false;
    }

    bool Chunk::deleteBuffers() {

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);

        return true;
    }

    glm::vec3 Chunk::getChunkPos() {

        return glm::vec3(this->posX, this->posY, this->posZ);
    }

    void Chunk::setChunkPos(int xp, int yp, int zp) {
        this->posX = xp;
        this->posZ = zp;
        this->posY = yp;
    }
    

    void Chunk::setShaderMat4(char* attribute, glm::mat4 matrix) {
        s1.setMat4(attribute, matrix);
     }




    float Chunk::d_below(glm::vec3 pos) {
        int x = (int)pos.x;
        int y = (int)pos.y;
        int z = (int)pos.z;

        int bp = x * y * z;
        float distance = 0.0f;

        while (blocks[bp] == 'A') {
            bp--;
            distance++;
        }


        return distance;
    }

    float Chunk::d_blockFront(glm::vec3 pos) {

        return 0;
    }

    float Chunk::d_blockBehind(glm::vec3 pos) {
        return 0;
    }

    float Chunk::d_blockLeft(glm::vec3 pos) {
        return 0;
    }

    float Chunk::d_blockRight(glm::vec3 pos) {
        return 0;
    }
