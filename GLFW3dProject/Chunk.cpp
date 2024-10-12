#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

#include "shader_s.h"

#include "Chunk.h"
#include "SimplexNoise.h"
#include "glm/glm.hpp"
#include <vector>
#include "TextureManager.h"


using std::vector;

    int posX;
    int posZ;
    bool baked;
    bool chunkBuild = false;
    bool firstBake = false;
    size_t bSize = 4096;
    char* blocks;
   
    char* hashedBlocks;
    int vCount;
    int cCounter;
    int v_size;
    float* vertices;
    bool bGen;

    unsigned int VBO, VAO;

    vector<float> chunk_verts = {};

    Shader s1;
    unsigned int texture;

    typedef struct {

        TextureManager::blocks c_top;
        TextureManager::blocks c_bottom;
        TextureManager::blocks c_left;
        TextureManager::blocks c_right;
        TextureManager::blocks c_front;
        TextureManager::blocks c_back;

    } b_look;

    vector<b_look> block_table;

    /*
        Constructor for a 16x16x16 Chunk

    */
    Chunk::Chunk() {
        blocks = (char*)malloc(bSize * sizeof(char));
        baked = false;
        bGen = false;
    }

    Chunk::Chunk(const Chunk& copy) {
       
        *this = copy;
    }

    void Chunk::populateBlockTable() {

     

        block_table.push_back(b_look());
        block_table.at(block_table.size() - 1).c_top = TextureManager::DIRT;
        block_table.at(block_table.size() - 1).c_bottom = TextureManager::DIRT;
        block_table.at(block_table.size() - 1).c_left = TextureManager::DIRT;
        block_table.at(block_table.size() - 1).c_right = TextureManager::DIRT;
        block_table.at(block_table.size() - 1).c_front = TextureManager::DIRT;
        block_table.at(block_table.size() - 1).c_back = TextureManager::DIRT;

        block_table.push_back(b_look());
        block_table.at(block_table.size() - 1).c_top = TextureManager::GRASS_TOP;
        block_table.at(block_table.size() - 1).c_bottom = TextureManager::DIRT;
        block_table.at(block_table.size() - 1).c_left = TextureManager::GRASS_SIDE;
        block_table.at(block_table.size() - 1).c_right = TextureManager::GRASS_SIDE;
        block_table.at(block_table.size() - 1).c_front = TextureManager::GRASS_SIDE;
        block_table.at(block_table.size() - 1).c_back = TextureManager::GRASS_SIDE;

        block_table.push_back(b_look());
        block_table.at(block_table.size() - 1).c_top = TextureManager::STONE;
        block_table.at(block_table.size() - 1).c_bottom = TextureManager::STONE;
        block_table.at(block_table.size() - 1).c_left = TextureManager::STONE;
        block_table.at(block_table.size() - 1).c_right = TextureManager::STONE;
        block_table.at(block_table.size() - 1).c_front = TextureManager::STONE;
        block_table.at(block_table.size() - 1).c_back = TextureManager::STONE;

        block_table.push_back(b_look());
        block_table.at(block_table.size() - 1).c_top = TextureManager::LOG_TOP;
        block_table.at(block_table.size() - 1).c_bottom = TextureManager::LOG;
        block_table.at(block_table.size() - 1).c_left = TextureManager::LOG;
        block_table.at(block_table.size() - 1).c_right = TextureManager::LOG;
        block_table.at(block_table.size() - 1).c_front = TextureManager::LOG;
        block_table.at(block_table.size() - 1).c_back = TextureManager::LOG;

        block_table.push_back(b_look());
        block_table.at(block_table.size() - 1).c_top = TextureManager::LEAF;
        block_table.at(block_table.size() - 1).c_bottom = TextureManager::LEAF;
        block_table.at(block_table.size() - 1).c_left = TextureManager::LEAF;
        block_table.at(block_table.size() - 1).c_right = TextureManager::LEAF;
        block_table.at(block_table.size() - 1).c_front = TextureManager::LEAF;
        block_table.at(block_table.size() - 1).c_back = TextureManager::LEAF;

        block_table.push_back(b_look());
        block_table.at(block_table.size() - 1).c_top = TextureManager::BEDROCK;
        block_table.at(block_table.size() - 1).c_bottom = TextureManager::BEDROCK;
        block_table.at(block_table.size() - 1).c_left = TextureManager::BEDROCK;
        block_table.at(block_table.size() - 1).c_right = TextureManager::BEDROCK;
        block_table.at(block_table.size() - 1).c_front = TextureManager::BEDROCK;
        block_table.at(block_table.size() - 1).c_back = TextureManager::BEDROCK;


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

    int Chunk::getLayeredNoise(int x, int z) {
        int yLevel = 0;

        
        int n1 = (int)((SimplexNoise::noise(x / 20, z / 20) + 1));
        int n2 = (int)((SimplexNoise::noise((x + 8) / 20, (z + 8) / 20) + 1));
        int n3 = (int)((SimplexNoise::noise((x + 16) / 20, (z + 16 ) / 20) + 1));

        int n4 = (int)((SimplexNoise::noise((x + 32) / 20, (z + 32) / 20) + 1));
        int n5 = (int)((SimplexNoise::noise((x + 48) / 20, (z + 48) / 20) + 1));
        int n6 = (int)((SimplexNoise::noise((x + 64) / 20, (z + 64) / 20) + 1));


        int n7 = (int)((SimplexNoise::noise(x / 10, z / 10) + 1));
        int n8 = (int)((SimplexNoise::noise((x + 8) / 10, (z + 8) / 10) + 1));
        int n9 = (int)((SimplexNoise::noise((x + 16) / 10, (z + 16) / 10) + 1));

        int n10 = (int)((SimplexNoise::noise((x + 32) / 10, (z + 32) / 10) + 1));
        int n11 = (int)((SimplexNoise::noise((x + 48) / 10, (z + 48) / 10) + 1));
        int n12 = (int)((SimplexNoise::noise((x + 64) / 10, (z + 64) / 10) + 1));
       

        int noise = (((n1 + n2 + n3 + n4 + n5 + n5 + n6 + n7) / 2) + ((n8 + n10 + n11 + n12) /2 ) + 2);
       
       

        return noise;
    }


    bool Chunk::firstChunkGen() {
      
        populateBlockTable();
        int position = 0;

        for (int x = 0; x < 64; x++)
        {
            for (int z = 0; z < 64; z++)
            {
                    int yLevel = 0;
                    int noise = getLayeredNoise(x + posX, z + posZ);
              
                    blocks[position] = 'B';
                    yLevel++;
                    position++;



                    for (int y = 1; y < noise -1; y++)
                    {
                      
                        if (y > 5) {
                            blocks[position] = 'D';
                        }
                        else {
                            blocks[position] = 'S';
                        }
                        yLevel++;
                        position++;
                     
                    }

                   
        
                    int rand = std::rand() % 1000;

                    /*
                    *   Tree Spawner 
                    * 
                    *
                        checks rand because spawn rate is 1/1000                                                            (spawn rate)
                        checks to see that position is > 2048 because we must edit block that is in front of current x 2
                        checks to see that position is < 63488 for the same reason                                          (min and max)
                        sets bound x > 3 && x < 61 && z > 3 && z < 61 so that tree does not spawn on chunk border           (spawn bound box)


                    */
                    if (rand < 2 && position > 2048 && position < 63488 && x > 3 && x < 61 && z > 3 && z < 61) {
                       
                        if (yLevel < 16) {
                            blocks[position] = 'D';
                            yLevel++;
                            position++;
                        }
                        for (size_t i = 0; i < 4; i++)
                        {

                            //overwritte, delete
                            if (i > 1) {
                                blocks[position - 16] = 'F';
                                blocks[position - 32] = 'F';

                                blocks[position + 16] = 'F';
                                blocks[position + 32] = 'F';

                                blocks[position + 1024] = 'F';
                                blocks[position + 2048] = 'F';

                                blocks[position - 1024] = 'F';
                                blocks[position - 2048] = 'F';


                            }
                            blocks[position] = 'L';
                            yLevel++;
                            position++;
                           
                        }
                        blocks[position] = 'F';
                        yLevel++;
                        position++;
                       

                        
                    }
                    else {
                        blocks[position] = 'G';
                        yLevel++;
                        position++;
                    }
                 

                   

                    if (yLevel < 16) {
                        
                        for (int i = yLevel; i < 16; i++) {
                          
                               
                                blocks[position] = 'A';
                                yLevel++;
                                position++;
                               
                            
                        }
                        
                    }
            }
        }

        /*
            Loops over all the blocks inorder to properly spawn the leaves of the trees
        */
        position = 0;
        for (int x = 0; x < 64; x++)
        {
            for (int z = 0; z < 64; z++)
            {
                int noise = getLayeredNoise(x + posX, z + posZ);

                for (int y = 0; y < 16; y++) {

                    if (blocks[position] == 'L') {
                        if (y > noise + 1) {
                            blocks[position - 16] = 'F';
                            blocks[position - 32] = 'F';

                            blocks[position + 16] = 'F';
                            blocks[position + 32] = 'F';

                            blocks[position + 1024] = 'F';
                            blocks[position + 2048] = 'F';

                            blocks[position - 1024] = 'F';
                            blocks[position - 2048] = 'F';


                            blocks[position + 1024 + 16] = 'F';
                            blocks[position + 2048 + 16] = 'F';
                            blocks[position + 1024 + 32] = 'F';
                            blocks[position + 2048 + 32] = 'F';

                            blocks[position + 1024 - 16] = 'F';
                            blocks[position + 2048 - 16] = 'F';
                            blocks[position + 1024 - 32] = 'F';
                            blocks[position + 2048 - 32] = 'F';

                            blocks[position - 1024 - 32] = 'F';
                            blocks[position - 2048 - 32] = 'F';
                            blocks[position - 1024 - 16] = 'F';
                            blocks[position - 2048 - 16] = 'F';

                            blocks[position - 1024 + 32] = 'F';
                            blocks[position - 2048 + 32] = 'F';
                            blocks[position - 1024 + 16] = 'F';
                            blocks[position - 2048 + 16] = 'F';


                         

                          
                        }
                    }
                  
                    if (blocks[position - 1] == 'L' && y == noise + 4 ) {
                        blocks[position] = 'F';
                        blocks[position + 1] = 'F';
                    }

                    if (blocks[position - 1] == 'L' && blocks[position] == 'F') {
                        blocks[position + 1024] = 'F';
                        blocks[position - 1024] = 'F';
                        blocks[position - 16] = 'F';
                        blocks[position + 16] = 'F';
                        blocks[position + 1024 + 16] = 'F';
                        blocks[position + 1024 - 16] = 'F';
                        blocks[position - 1024 - 16] = 'F';
                        blocks[position - 1024 + 16] = 'F';
                    }

                
                  
                    position++;

               }
            }
        }

        return true;
    }

    /*
        Helper function which combindes face vertex array and face texture coordinate vector and pushes the vertices to chunk_verts
    */

    void Chunk::pushToVertices(float eArray[], std::vector<float> tc) {

        int texc = 0;
        int tIndex = 0;
        int index = 0;
        for (int i = 0; i < 48; i++)
        {

            if (texc == 3 || texc == 4) {
                chunk_verts.push_back(tc.at(tIndex));
                tIndex++;  
            }
            else {

                chunk_verts.push_back(eArray[index]);
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
    bool Chunk::genChunkMesh() {
        TextureManager tf = TextureManager(5);
        cCounter = 0;
        for (int x = 0; x < 64; x++) {
            for (int z = 0; z < 64; z++) { 
                for (int y = 0; y < 16; y++) {
                            int noise = getLayeredNoise(x + posX, z + posZ);
                   
                           b_look fb;
                           if (blocks[cCounter] == 'D')
                               fb = block_table.at(0);
                           if (blocks[cCounter] == 'G')
                               fb = block_table.at(1);
                           if (blocks[cCounter] == 'S')
                               fb = block_table.at(2);
                           if (blocks[cCounter] == 'L')
                               fb = block_table.at(3);
                           if (blocks[cCounter] == 'F')
                               fb = block_table.at(4);
                           if (blocks[cCounter] == 'B')
                               fb = block_table.at(5);
   
                          
                               int noiseLeft = getLayeredNoise(x + posX, z + posZ - 1);
                               if ((z == 0 && y < noise && y >= noiseLeft) || (blocks[cCounter] != 'A' && (cCounter > 15 && blocks[cCounter - 16] == 'A'))) {
                                   //left face
                                   // v1, v2, v3, t1, t2, n1, n2 ,n3
                                   float left_face[] = { -0.5f + x + posX,    -0.5f + y,     -0.5f + z + posZ,          0.0f, 0.0f, -1.0f,
                                                            0.5f + x + posX,    -0.5f + y,     -0.5f + z + posZ,          0.0f, 0.0f, -1.0f,
                                                            0.5f + x + posX,     0.5f + y,     -0.5f + z + posZ,          0.0f, 0.0f, -1.0f,

                                                            0.5f + x + posX,     0.5f + y,     -0.5f + z + posZ,          0.0f, 0.0f, -1.0f,
                                                           -0.5f + x + posX,    0.5f + y,     -0.5f + z + posZ,          0.0f, 0.0f, -1.0f,
                                                           -0.5f + x + posX,   -0.5f + y,     -0.5f + z + posZ,          0.0f, 0.0f, -1.0f

                                   };
                                   pushToVertices(left_face, tf.getFaceCoords(fb.c_left, TextureManager::LEFT));
                                   vCount += 6;
                               }

                               int noiseRight = getLayeredNoise(x + posX, z + posZ + 1);
                               if ((z == 63 && y < noise && y >= noiseRight) || (blocks[cCounter] != 'A' && cCounter < bSize && blocks[cCounter + 16] == 'A')) {
                                   //right face
                                   //z=0
                                   float right_face[] = { -0.5f + x + posX,   -0.5f + y,     0.5f + z + posZ,      0.0f, 0.0f, 1.0f,
                                                             0.5f + x + posX,   -0.5f + y,     0.5f + z + posZ,      0.0f, 0.0f, 1.0f,
                                                             0.5f + x + posX,    0.5f + y,     0.5f + z + posZ,      0.0f, 0.0f, 1.0f,

                                                             0.5f + x + posX,    0.5f + y,     0.5f + z + posZ,      0.0f, 0.0f, 1.0f,
                                                            -0.5f + x + posX,    0.5f + y,     0.5f + z + posZ,      0.0f, 0.0f, 1.0f,
                                                            -0.5f + x + posX,   -0.5f + y,     0.5f + z + posZ,      0.0f, 0.0f, 1.0f

                                   };

                                   pushToVertices(right_face, tf.getFaceCoords(fb.c_right, TextureManager::RIGHT));
                                   vCount += 6;
                               }

                               //front
                               // x = 63
                               int noiseFront = getLayeredNoise(x + posX + 1, z + posZ);
                               if ((x == 63 && y < noise && y >= noiseFront) || (blocks[cCounter] != 'A' && cCounter < (bSize - 1023) && blocks[cCounter + 1024] == 'A')) {
                                   float front_face[] = { 0.5f + x + posX,    0.5f + y,     0.5f + z + posZ,       1.0f, 0.0f, 0.0f,
                                                             0.5f + x + posX,    0.5f + y,    -0.5f + z + posZ,       1.0f, 0.0f, 0.0f,
                                                             0.5f + x + posX,   -0.5f + y,    -0.5f + z + posZ,       1.0f, 0.0f, 0.0f,

                                                             0.5f + x + posX,   -0.5f + y,    -0.5f + z + posZ,       1.0f, 0.0f, 0.0f,
                                                             0.5f + x + posX,   -0.5f + y,     0.5f + z + posZ,       1.0f, 0.0f, 0.0f,
                                                             0.5f + x + posX,    0.5f + y,     0.5f + z + posZ,       1.0f, 0.0f, 0.0f

                                   };
                                   pushToVertices(front_face, tf.getFaceCoords(fb.c_front, TextureManager::FRONT));
                                   vCount += 6;
                               }
                               //back
                               // x = 0
                               int noiseBack = getLayeredNoise(x + posX - 1, z + posZ);
                               if ((x == 0 && y < noise && y >= noiseBack) || (blocks[cCounter] != 'A' && cCounter > 1023 && blocks[cCounter - 1024] == 'A')) {
                                   float back_face[] = { -0.5f + x + posX,    0.5f + y,     0.5f + z + posZ,       -1.0f, 0.0f, 0.0f,
                                                           -0.5f + x + posX,    0.5f + y,    -0.5f + z + posZ,       -1.0f, 0.0f, 0.0f,
                                                           -0.5f + x + posX,   -0.5f + y,    -0.5f + z + posZ,       -1.0f, 0.0f, 0.0f,

                                                           -0.5f + x + posX,   -0.5f + y,    -0.5f + z + posZ,       -1.0f, 0.0f, 0.0f,
                                                           -0.5f + x + posX,   -0.5f + y,     0.5f + z + posZ,       -1.0f, 0.0f, 0.0f,
                                                           -0.5f + x + posX,    0.5f + y,     0.5f + z + posZ,       -1.0f, 0.0f, 0.0f

                                   };
                                   pushToVertices(back_face, tf.getFaceCoords(fb.c_back, TextureManager::BACK));
                                   vCount += 6;
                               }
                               if (blocks[cCounter] != 'A' && cCounter < bSize && blocks[cCounter + 1] == 'A') {
                                   //top
                                   float top_face[] = { -0.5f + x + posX,    0.5f + y,    -0.5f + z + posZ,        0.0f, 1.0f, 0.0f,
                                                         0.5f + x + posX,    0.5f + y,    -0.5f + z + posZ,        0.0f, 1.0f, 0.0f,
                                                         0.5f + x + posX,    0.5f + y,     0.5f + z + posZ,        0.0f, 1.0f, 0.0f,

                                                         0.5f + x + posX,    0.5f + y,     0.5f + z + posZ,        0.0f, 1.0f, 0.0f,
                                                        -0.5f + x + posX,    0.5f + y,     0.5f + z + posZ,        0.0f, 1.0f, 0.0f,
                                                        -0.5f + x + posX,    0.5f + y,    -0.5f + z + posZ,        0.0f, 1.0f, 0.0f

                                   };
                                   pushToVertices(top_face, tf.getFaceCoords(fb.c_top, TextureManager::TOP));
                                   vCount += 6;
                               }
                               //bottom
                               if (blocks[cCounter] != 'A' && cCounter > 0 && blocks[cCounter - 1] == 'A') {
                                   float bottom_face[] = { -0.5f + x + posX,   -0.5f + y,    -0.5f + z + posZ,       0.0f, -1.0f, 0.0f,
                                                             0.5f + x + posX,   -0.5f + y,    -0.5f + z + posZ,       0.0f, -1.0f, 0.0f,
                                                             0.5f + x + posX,   -0.5f + y,     0.5f + z + posZ,       0.0f, -1.0f, 0.0f,

                                                             0.5f + x + posX,   -0.5f + y,     0.5f + z + posZ,       0.0f, -1.0f, 0.0f,
                                                            -0.5f + x + posX,   -0.5f + y,     0.5f + z + posZ,       0.0f, -1.0f, 0.0f,
                                                            -0.5f + x + posX,   -0.5f + y,    -0.5f + z + posZ,       0.0f, -1.0f, 0.0f

                                   };
                                   pushToVertices(bottom_face, tf.getFaceCoords(fb.c_bottom, TextureManager::BOTTOM));
                                   vCount += 6;
                               }
                           

                           
                          
                         
                       
                        cCounter++;
                }
            }
        
        }

       baked = true;
       return true;
    }

    void Chunk::generateBufferData() {

        vertices = &chunk_verts[0];



        glGenVertexArrays(1, &VAO);

        glGenBuffers(1, &VBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vCount * 32, vertices, GL_STATIC_DRAW);


        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
        glEnableVertexAttribArray(2);

        
       
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

    
        if (baked) {

            if (!bGen) {
               // loadTexture();
                s1 = Shader("chunk_texture.vs", "chunk_texture.fs");
              
                generateBufferData();
                bGen = true;
            }

          
            s1.use();
            s1.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
            s1.setVec3("lightPos", cameraPos + glm::vec3(40.0f, 40.0f, 40.0f));
            s1.setVec3("viewPos", cameraPos);

            s1.setMat4("model", model);

            s1.setMat4("projection", projection);
            s1.setMat4("view", view);

            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, vCount);

        }
       

        return false;
    }

    bool Chunk::deleteBuffers() {

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);

        return true;
    }

    glm::vec2 Chunk::getChunkPos() {

        return glm::vec2(posX, posZ);
    }

    void Chunk::setChunkPos(int xp, int zp) {
        this->posX = xp;
        this->posZ = zp;
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
