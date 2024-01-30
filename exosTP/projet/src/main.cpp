#include <iostream>
#include <fstream>
#include <sstream> 
#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stbimage/stb_image.h>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glengine/orbitalCamera.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "projet/config.hpp" 

//=========SETUP=CAMERA=========
bool firstMouse = true;
float lastX;
float lastY;

enum class MousePressedButton { NONE, LEFT, RIGHT, MIDDLE };
MousePressedButton mouseButtonState = MousePressedButton::NONE;
// La camera
GLEngine::OrbitalCamera orbitalCamera(glm::vec3(0.3f, 0.4f, 3.0f), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

void onMouseButton(GLFWwindow* window, int button, int action, int mods) {
  if(!ImGui::GetIO().WantCaptureMouse){
    if (action == GLFW_RELEASE) {
      mouseButtonState = MousePressedButton::NONE;
    }
    else {
      switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT: mouseButtonState = MousePressedButton::LEFT;
          break;
        case GLFW_MOUSE_BUTTON_RIGHT: mouseButtonState = MousePressedButton::RIGHT;
          break;
        case GLFW_MOUSE_BUTTON_MIDDLE: mouseButtonState = MousePressedButton::MIDDLE;
          break;
      }
    }
  }
  
}

void onMouseMove(GLFWwindow* window, double xpos, double ypos) {
  if (mouseButtonState == MousePressedButton::NONE) {
    lastX = (float)xpos;
    lastY = (float)ypos;
  }
  else {
    if (firstMouse) {
      lastX = xpos;
      lastY = ypos;
      firstMouse = false;
    }

    float xoffset = (float)xpos - lastX;
    float yoffset = lastY - (float)ypos;

    lastX = (float)xpos;
    lastY = (float)ypos;

    switch (mouseButtonState) {
      case MousePressedButton::LEFT: orbitalCamera.orbit(xoffset, yoffset);
        break;
      case MousePressedButton::RIGHT:
        orbitalCamera.track(xoffset);
        orbitalCamera.pedestal(yoffset);
        break;
      case MousePressedButton::MIDDLE: orbitalCamera.dolly(yoffset);
        break;
    }
  }
}

void processInput(GLFWwindow *window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
};
//=========DEBUG=FUNCTION=========
void printV(std::vector<float> v){
  int cmp = 0;
  for(float n: v){
    std::cout << n << " | ";
    cmp++;
    if(cmp%2 == 0){
      std::cout << std::endl;
    }
  }
    
}
void printV2(std::vector<unsigned int> v){
  for(unsigned int n: v)
    std::cout << n << std::endl;
}
//=========MODEL===========
void loadModel(std::string link, std::vector<float> &vertices, std::vector<unsigned int> &indices, std::vector<float> &normal, std::vector<float> &texture){
  std::ifstream fileModel(std::string(_resources_directory).append("models/"+link));
  float buffFloat;
  unsigned int buffIndice;
  std::string buffValue2;
  if(fileModel.is_open()){
    while(fileModel){
      fileModel >> buffValue2;
      if(buffValue2 == "v"){
        for(int i = 0; i < 3; i++){
          fileModel >> buffFloat;
          vertices.push_back(buffFloat);
        }
      }
      if(buffValue2 == "f"){
        for(int i = 0; i < 3; i++){
          fileModel >> buffIndice;
          indices.push_back(buffIndice-1);
        }
      }
      if(buffValue2 == "vn"){
        for(int i = 0; i < 3; i++){
          fileModel >> buffFloat;
          normal.push_back(buffFloat);
        }
      }
      if(buffValue2 == "vt"){
        for(int i = 0; i < 2; i++){
          fileModel >> buffFloat;
          texture.push_back(buffFloat);
        }
      }          
    }
    if(texture.size()){
      texture.pop_back();
      texture.pop_back();
    }   
  }
}
//=========NORMAL===========
void computeNormal(const std::vector<float>& vertices,
                   const std::vector<unsigned int>& indices,
                   std::vector<float>& normal) {
    // init normal vector
    for (std::size_t k = 0; k < vertices.size(); k++) {
        normal.push_back(0);
    }

    // Compute normal
    for (unsigned int k = 0; k < indices.size() - 3; k += 3) {
        unsigned int i1 = indices[k];
        unsigned int i2 = indices[k+1];
        unsigned int i3 = indices[k+2];

        // Vertices
        glm::vec3 v1 = glm::vec3(vertices[3*i1], vertices[3*i1 + 1], vertices[3*i1 + 2]);
        glm::vec3 v2 = glm::vec3(vertices[3*i2], vertices[3*i2 + 1], vertices[3*i2 + 2]);
        glm::vec3 v3 = glm::vec3(vertices[3*i3], vertices[3*i3 + 1], vertices[3*i3 + 2]);

        // Compute normal
        glm::vec3 n = glm::cross(v3 - v1, v3 - v2);

        for (std::size_t di = 0; di < 3; di++) {
            normal[3 * i1 + di] += n[di];
            normal[3 * i2 + di] += n[di];
            normal[3 * i3 + di] += n[di];
        }
    }

    // Normalize
    for (std::size_t k = 0; k < normal.size() - 3; k += 3) {
        glm::vec3 n = glm::normalize(glm::vec3(normal[k], normal[k + 1], normal[k + 2]));
        normal[k] = n[0];
        normal[k+1] = n[1];
        normal[k+2] = n[2];
    }
}

std::vector<float> mixVerticesNormal(const std::vector<float> vertices, const std::vector<float> normal, const std::vector<float> texture){
  std::vector<float> res;
  size_t cmpText = 0;
  for(size_t i = 0; i < vertices.size(); i+=3){
    res.push_back(vertices[i]);
    res.push_back(vertices[i+1]);
    res.push_back(vertices[i+2]);
    res.push_back(normal[i]);
    res.push_back(normal[i+1]);
    res.push_back(normal[i+2]);
    if(texture.size()){
      res.push_back(texture[cmpText]);
      res.push_back(texture[cmpText+1]);
      cmpText += 2;
    }
  }
  return res;
}
//=========SHADER==========
void createShader(std::string linkVert, std::string linkFrag, unsigned int& sp){
  unsigned int vs, fs;
  int  success;
  char infoLog[512];
  //CREATE=V=SHADER
  std::string code;
  std::ifstream file;
  std::stringstream iss;
  file.open(std::string(_resources_directory).append(linkVert));
  iss << file.rdbuf();
  file.close();
  code = iss.str();
  const char *vertexShaderSource = code.c_str();
//CREATE=F=SHADER
  std::string code1;
  std::ifstream file1;
  std::stringstream iss1;
  file1.open(std::string(_resources_directory).append(linkFrag));
  iss1 << file1.rdbuf();
  file1.close();
  code1 = iss1.str();
  const char *fragmentShaderSource = code1.c_str();
//VERTEX=SHADER
  vs = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vs, 1, &vertexShaderSource, NULL);
  glCompileShader(vs);
  glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
  if(!success){
      glGetShaderInfoLog(vs, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  }
//FRAGMENT=SHADER
  fs = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fs, 1, &fragmentShaderSource, NULL);
  glCompileShader(fs);
  glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
  if(!success){
      glGetShaderInfoLog(fs, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  }
//LINK=SHADERS
  sp = glCreateProgram();
  glAttachShader(sp, vs);
  glAttachShader(sp, fs);
  glLinkProgram(sp);
  if (!success) {
      glGetProgramInfoLog(sp, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
  }
  glDeleteShader(vs);
  glDeleteShader(fs);
}

int main(int argc, char* argv[]) {
  // ça part de là !
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  GLFWwindow* window = glfwCreateWindow(1000, 500, "je suis une buuuuulle2", NULL, NULL);
  if (window == NULL){
      std::cout << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
      return -1;
  }
  //setup camera
  glfwSetMouseButtonCallback(window, onMouseButton);
  glfwSetCursorPosCallback(window, onMouseMove);
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
      std::cout << "Failed to initialize GLAD" << std::endl;
      return -1;
  } 
//=========SETUP=GUI=========
IMGUI_CHECKVERSION();
ImGui::CreateContext();
ImGuiIO& io = ImGui::GetIO();
(void)io;
io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
ImGui::StyleColorsDark();
ImGui_ImplGlfw_InitForOpenGL(window, true);
ImGui_ImplOpenGL3_Init();
//=========GLOBAL=STATE=========
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_STENCIL_TEST);
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
//=========CREATE=SHADER=========
  //SIMPLE
  unsigned int shaderProgramS;
  createShader("shaders/simple/simple.vert", "shaders/simple/simple.frag",shaderProgramS);
  //DAMIER
  unsigned int shaderProgramD;
  createShader("shaders/simple/texture.vert", "shaders/simple/damier.frag",shaderProgramD);
  //NPR
  unsigned int shaderProgramNPR,shaderProgramOutline;
  createShader("shaders/npr/simple.vert", "shaders/npr/simple.frag",shaderProgramNPR);
  createShader("shaders/npr/outline.vert", "shaders/npr/outline.frag",shaderProgramOutline);
  //Phong
  unsigned int shaderProgramP;
  createShader("shaders/light/phong.vert", "shaders/light/phong.frag",shaderProgramP);
//========MODEL=========
  std::vector<float> vertices;
  std::vector<unsigned int> indices;
  std::vector<float> normal;
  std::vector<float> texture;

  std::string nameFile = argv[1];
  
  loadModel(nameFile, vertices, indices, normal, texture);
  if(!normal.size()){
    std::cout << "The normal are getting computed"<< std::endl;
    computeNormal(vertices, indices, normal);
  }
  std::vector<float> test = mixVerticesNormal(vertices, normal, texture);
//========VERTEX=DATA=========
  unsigned int VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, test.size()*sizeof(float), test.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
//=========VERTEX=========

  if(texture.size()){
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); 
  //=========NORMAL=========
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
  //=========TEXTURE COOR=========
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(3);
  }
  else{
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); 

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
  }
//========VALUE=GUI=======
bool NPR = false;
bool displayContour = true;
unsigned int shaderProgram = shaderProgramS;
float as, sp = 0.0f;
glm::vec3 colorModel, colorBorder;
glm::vec3 colorLight = glm::vec3(1.0f,1.0f,1.0f);
glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 0.0f);
//=========RENDER=========
  while(!glfwWindowShouldClose(window)){
      processInput(window);

    //SETUP GUI
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();
      ImGui::Text("Model");
      ImGui::ColorEdit3("Color", (float*)&colorModel);
      ImGui::ColorEdit3("Light Color", (float*)&colorLight);
      ImGui::SliderFloat3("Light Position",(float*)&lightPos,-1,1,"%.3f");
      if(ImGui::CollapsingHeader("NPR")){
        ImGui::Checkbox("Contour", &displayContour);
        ImGui::ColorEdit3("Border Color", (float*)&colorBorder);
        NPR = true;
        shaderProgram = shaderProgramNPR;
      }
      else if(ImGui::CollapsingHeader("Phong")){
        ImGui::SliderFloat("Ambient Strength",&as,0,1,"%.3f");
        ImGui::SliderFloat("Specular",&sp,0,150,"%.3f");
        NPR = false;
        shaderProgram = shaderProgramP;
      }
      else if(texture.size() && ImGui::CollapsingHeader("Damier")){
        NPR = false;
        shaderProgram = shaderProgramD;
      }
      else{
        NPR = false;
        shaderProgram = shaderProgramS;
      }
        

      ImGui::Render();
      ImGui::EndFrame();

      glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

      

      glUseProgram(shaderProgram);
    //SETUP LUMIERE
    
      
      glUniform3fv(glGetUniformLocation(shaderProgram, "lightColor"), 1, glm::value_ptr(colorLight));
      glUniform3fv(glGetUniformLocation(shaderProgram, "lightPos"), 1, glm::value_ptr(lightPos));
      glUniform3fv(glGetUniformLocation(shaderProgram, "cameraPos"), 1, glm::value_ptr(camPos));
      
      glUniform3fv(glGetUniformLocation(shaderProgram, "modelColor"),1, glm::value_ptr(colorModel));
      glUniform1f(glGetUniformLocation(shaderProgram, "as"), as);
      glUniform1f(glGetUniformLocation(shaderProgram, "sp"), sp);

    //SETUP CAMERA
      glm::mat4 model         = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
      glm::mat4 view          = glm::mat4(1.0f);
      glm::mat4 projection    = glm::mat4(1.0f);
      int w, h;
      glfwGetFramebufferSize(window,&w,&h);
//(float)glfwGetTime() * 
      model = glm::rotate(model, glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
      view  = orbitalCamera.getViewMatrix();
      projection = glm::perspective(glm::radians(45.0f), (float)w / (float)h, 0.5f, 100.0f);

      glUseProgram(shaderProgramOutline);
      int modelLoc = glGetUniformLocation(shaderProgramOutline, "model");
      glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

      int viewLoc = glGetUniformLocation(shaderProgramOutline, "view");
      glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

      int projLoc = glGetUniformLocation(shaderProgramOutline, "projection");
      glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

      glUseProgram(shaderProgram);
      modelLoc = glGetUniformLocation(shaderProgram, "model");
      glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

      viewLoc = glGetUniformLocation(shaderProgram, "view");
      glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

      projLoc = glGetUniformLocation(shaderProgram, "projection");
      glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


    //DRAW MODEL
      //là c'est un display nrml
      glStencilFunc(GL_ALWAYS, 1, 0xFF);
      glStencilMask(0xFF);
      //model
      glBindVertexArray(VAO);
      glUseProgram(shaderProgram);
      glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
      //là c'est un display du contour
      glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
      glStencilMask(0x00);
      glDisable(GL_DEPTH_TEST);
      //model outlined
      glUseProgram(shaderProgramOutline);
      glUniform1f(glGetUniformLocation(shaderProgramOutline, "outlining"), 0.01f);
      glUniform3fv(glGetUniformLocation(shaderProgramOutline, "borderColor"),1, glm::value_ptr(colorBorder));
      glBindVertexArray(VAO);
      if(displayContour && NPR){
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
      }
      

      glStencilMask(0xFF);
      glStencilFunc(GL_ALWAYS, 0, 0xFF);
      glEnable(GL_DEPTH_TEST);

      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
      glfwSwapBuffers(window);
      glfwPollEvents();    
  }
//=====RELACHE=MEMOIRE====
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteProgram(shaderProgram);
  glDeleteProgram(shaderProgramOutline);

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
//=========Fin===========
  glfwTerminate();

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  return 0;
}
