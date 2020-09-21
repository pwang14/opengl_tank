#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "shader.h"
#include "camera.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void processInput(GLFWwindow* window);

float colorF(int value) {
	return value / 255.0f;
}

glm::vec3 arrToVec3(float* arr) {
	return glm::vec3(arr[0], arr[1], arr[2]);
}

bool outOfBounds();

const int windowWidth = 1920, windowHeight = 1080;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

float lastX = windowWidth / 2.0f;
float lastY = windowHeight / 2.0f;

bool firstMouse = true;

Camera camera(glm::vec3(0.0f, 0.5f, 0.0f));

glm::vec3 lightPos(0.0f, 3.0f, 0.0f);

float tankYaw = 90.0f;
glm::vec3 tankYawVec = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 tankPos = glm::vec3(0.0f, 0.0f, 0.0f);

float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

float tank_transforms[] = {
	 1.375f,  0.5f, 0.0f,	0.0f, 0.0f, 1.0f, 0.0f,	0.75f, 1.0f,   4.0f,
	-1.375f,  0.5f, 0.0f,	0.0f, 0.0f, 1.0f, 0.0f, 0.75f, 1.0f,   4.0f,
	 0.0f,    1.0f, 0.0f,	0.0f, 0.0f, 1.0f, 0.0f, 2.0f,  1.0f,   3.0f,
	 0.0f,    2.0f, 0.0f,	0.0f, 0.0f, 1.0f, 0.0f, 1.5f,  1.0f,   1.5f,
	 0.0f,    2.0f, 1.75f,	0.0f, 0.0f, 1.0f, 0.0f, 0.25f, 0.25f,  2.0f
};

int main(int argc, char* argv[]) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "tank_test", NULL, NULL);
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glViewport(0, 0, windowWidth, windowHeight);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);

	float floor_vertices[] = {
		 10.0f, 0.0f,  10.0f, 0.0f, 1.0f, 0.0f,
		 10.0f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f,
		-10.0f, 0.0f,  10.0f, 0.0f, 1.0f, 0.0f,
		-10.0f, 0.0f,  10.0f, 0.0f, 1.0f, 0.0f,
		 10.0f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f,
		-10.0f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f
	};

	float floor_transforms[] = {
		 10.0f, 5.0f,   0.0f,	90.0f,	0.0f, 0.0f,  1.0f, 0.5f, 1.0f, 1.0f,
		  0.0f, 5.0f, -10.0f,	90.0f,	1.0f, 0.0f,  0.0f, 1.0f, 1.0f, 0.5f,
		-10.0f, 5.0f,   0.0f,	90.0f,	0.0f, 0.0f, -1.0f, 0.5f, 1.0f, 1.0f,
		  0.0f, 5.0f,  10.0f,	90.0f, -1.0f, 0.0f,  0.0f, 1.0f, 1.0f, 0.5f
	};

	float tank_colors[] = {
		colorF(66), colorF(66), colorF(66),
		colorF(66), colorF(66), colorF(66),
		colorF(251), colorF(140), 0.0f,
		colorF(245), colorF(124), 0.0f,
		colorF(245), colorF(124), 0.0f
	};

	unsigned int fVBO, fVAO;

	glGenVertexArrays(1, &fVAO);
	glBindVertexArray(fVAO);

	glGenBuffers(1, &fVBO);
	glBindBuffer(GL_ARRAY_BUFFER, fVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floor_vertices), floor_vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	unsigned int VBO, VAO;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	unsigned int lightVAO;

	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat3 normModel = glm::mat3(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), ((float)windowWidth) / windowHeight, 0.1f, 100.0f);

	Shader shader("shader.vs", "shader.fs");
	Shader lightShader("shader.vs", "light_shader.fs");
	Shader shadowShader("shadow_shader.vs", "shadow_shader.fs");

	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(colorF(227), colorF(242), colorF(253), 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();

		shader.setVec3f("lightColor", 1.0f, 1.0f, 1.0f);
		shader.setVec3("lightPos", lightPos);
		shader.setVec3("viewPos", camera.position);

		view = camera.getViewMatrix();
		shader.setMat4("view", view);
		projection = glm::perspective(glm::radians(camera.fov), ((float)windowWidth) / windowHeight, 0.1f, 100.0f);
		shader.setMat4("projection", projection);

		//_____FLOOR_____

		glBindVertexArray(fVAO);

		shader.setFloat("specularStrength", 0.0f);
		shader.setFloat("flatness", 0.75f);
		shader.setVec4f("objColor", colorF(48), colorF(63), colorF(159), 1.0f);
		model = glm::mat4(1.0f);
		shader.setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		//_____WALLS______

		shader.setFloat("flatness", 0.9f);

		for (int i = 0; i < 4; ++i) {
			if (i % 2 == 0) shader.setVec4f("objColor", colorF(21), colorF(101), colorF(192), 1.0f);
			else shader.setVec4f("objColor", colorF(25), colorF(118), colorF(210), 1.0f);
			model = glm::mat4(1.0f);
			model = glm::translate(model, arrToVec3(floor_transforms + 10 * i));
			model = glm::rotate(model, glm::radians(floor_transforms[10 * i + 3]), arrToVec3(floor_transforms + 10 * i + 4));
			model = glm::scale(model, arrToVec3(floor_transforms + 10 * i + 7));
			shader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		//_____TANK_____

		glBindVertexArray(VAO);

		shader.setFloat("specularStrength", 0.5f);
		shader.setFloat("flatness", 0.25f);

		for (int i = 0; i < 5; ++i) {
			shader.setVec4f("objColor", tank_colors[3 * i], tank_colors[3 * i + 1], tank_colors[3 * i + 2], 1.0f);
			model = glm::mat4(1.0f);
			model = glm::translate(model, tankPos);
			model = glm::rotate(model, glm::radians(90.0f - tankYaw), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.5f));
			model = glm::translate(model, arrToVec3(tank_transforms + 10 * i));
			model = glm::scale(model, arrToVec3(tank_transforms + 10 * i + 7));
			shader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//_____SHADOWS______

		shadowShader.use();
		shadowShader.setFloat("shadowStrength", 0.5f);
		shadowShader.setMat4("view", view);
		shadowShader.setMat4("projection", projection);

		shadowShader.setVec3("lightPos", lightPos);

		for (int i = 0; i < 5; ++i) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, tankPos);
			model = glm::rotate(model, glm::radians(90.0f - tankYaw), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.5f));
			model = glm::translate(model, arrToVec3(tank_transforms + 10 * i));
			model = glm::scale(model, arrToVec3(tank_transforms + 10 * i + 7));
			shadowShader.setMat4("model", model);

			shadowShader.setBool("rightHand", true);
			shadowShader.setMat4("planeTransform", glm::mat4(1.0f));
			shadowShader.setVec3f("baseColor", colorF(40), colorF(53), colorF(147));

			for (int j = 0; j < 2; ++j) {
				shadowShader.setFloat("offset", j * 0.001f);
				shadowShader.setVec3f("inP1", floor_vertices[18 * j], floor_vertices[18 * j + 1], floor_vertices[18 * j + 2]);
				shadowShader.setVec3f("inP2", floor_vertices[18 * j + 6], floor_vertices[18 * j + 7], floor_vertices[18 * j + 8]);
				shadowShader.setVec3f("inP3", floor_vertices[18 * j + 12], floor_vertices[18 * j + 13], floor_vertices[18 * j + 14]);

				glDrawArrays(GL_TRIANGLES, 0, 36);
			}

			for (int i = 0; i < 4; ++i) {
				if (i % 2 == 0) shader.setVec4f("objColor", colorF(1), colorF(81), colorF(172), 1.0f);
				else shader.setVec4f("objColor", colorF(5), colorF(98), colorF(190), 1.0f);
				glm::mat4 planeTransform = glm::mat4(1.0f);
				planeTransform = glm::translate(planeTransform, arrToVec3(floor_transforms + 10 * i));
				planeTransform = glm::rotate(planeTransform, glm::radians(floor_transforms[10 * i + 3]), arrToVec3(floor_transforms + 10 * i + 4));
				planeTransform = glm::scale(planeTransform, arrToVec3(floor_transforms + 10 * i + 7));
				shadowShader.setMat4("planeTransform", planeTransform);

				for (int j = 0; j < 2; ++j) {
					shadowShader.setFloat("offset", j * 0.001f);
					shadowShader.setVec3f("inP1", floor_vertices[18 * j], floor_vertices[18 * j + 1], floor_vertices[18 * j + 2]);
					shadowShader.setVec3f("inP2", floor_vertices[18 * j + 6], floor_vertices[18 * j + 7], floor_vertices[18 * j + 8]);
					shadowShader.setVec3f("inP3", floor_vertices[18 * j + 12], floor_vertices[18 * j + 13], floor_vertices[18 * j + 14]);

					glDrawArrays(GL_TRIANGLES, 0, 36);
				}
			}
		}

		for (int i = 2; i < 5; ++i) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, tankPos);
			model = glm::rotate(model, glm::radians(90.0f - tankYaw), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.5f));
			model = glm::translate(model, arrToVec3(tank_transforms + 10 * i));
			model = glm::scale(model, arrToVec3(tank_transforms + 10 * i + 7));
			shadowShader.setMat4("model", model);

			glm::mat4 planeTransform;
			shadowShader.setVec3f("baseColor", colorF(191), colorF(54), colorF(12));
			shadowShader.setBool("rightHand", false);

			for (int j = 0; j < 5; ++j) {
				if (j != 3 && j < i) {
					if (j > 1) shadowShader.setVec3f("baseColor", colorF(209), colorF(78), colorF(0));
					else shadowShader.setVec3f("baseColor", colorF(50), colorF(50), colorF(50));

					planeTransform = glm::mat4(1.0f);
					planeTransform = glm::translate(planeTransform, tankPos);
					planeTransform = glm::rotate(planeTransform, glm::radians(90.0f - tankYaw), glm::vec3(0.0f, 1.0f, 0.0f));
					planeTransform = glm::scale(planeTransform, glm::vec3(0.5f));
					planeTransform = glm::translate(planeTransform, arrToVec3(tank_transforms + 10 * j));
					planeTransform = glm::scale(planeTransform, arrToVec3(tank_transforms + 10 * j + 7));
					shadowShader.setMat4("planeTransform", planeTransform);

					for (int k = 10; k < 12; ++k) {
						shadowShader.setFloat("offset", (k + 2 * j) * 0.0001f);
						shadowShader.setVec3f("inP1", vertices[18 * k], vertices[18 * k + 1], vertices[18 * k + 2]);
						shadowShader.setVec3f("inP2", vertices[18 * k + 6], vertices[18 * k + 7], vertices[18 * k + 8]);
						shadowShader.setVec3f("inP3", vertices[18 * k + 12], vertices[18 * k + 13], vertices[18 * k + 14]);

						glDrawArrays(GL_TRIANGLES, 0, 36);
					}
				}
			}
		}

		//_____LIGHT_____

		lightShader.use();

		lightShader.setMat4("view", view);
		lightShader.setMat4("projection", projection);

		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		lightShader.setMat4("model", model);

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.updateDirection(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.updateFov(yoffset);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);

	float speedAdjust = deltaTime;
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) speedAdjust *= 2;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		glm::vec3 prevTankPos = tankPos;
		tankPos = tankPos + camera.movementSpeed * speedAdjust * tankYawVec;
		if (outOfBounds()) tankPos = prevTankPos;
		else camera.updatePosition(FORWARD, speedAdjust, tankYawVec);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		glm::vec3 prevTankPos = tankPos;
		tankPos = tankPos - camera.movementSpeed * speedAdjust * tankYawVec;
		if (outOfBounds()) tankPos = prevTankPos;
		else camera.updatePosition(BACKWARD, speedAdjust, tankYawVec);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		float prevTankYaw = tankYaw;
		tankYaw -= 75.0f * speedAdjust;
		if (outOfBounds()) tankYaw = prevTankYaw;
		else tankYawVec = glm::vec3(cos(glm::radians(tankYaw)), 0.0f, sin(glm::radians(tankYaw)));

	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		float prevTankYaw = tankYaw;
		tankYaw += 75.0f * speedAdjust;
		if (outOfBounds()) tankYaw = prevTankYaw;
		else tankYawVec = glm::vec3(cos(glm::radians(tankYaw)), 0.0f, sin(glm::radians(tankYaw)));
	}
	//if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) camera.updatePosition(UP, speedAdjust);
	//if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) camera.updatePosition(DOWN, speedAdjust);
}

bool outOfBounds() {
	for (int i = 0; i < 5; ++i) {
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, tankPos);
		model = glm::rotate(model, glm::radians(90.0f - tankYaw), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		model = glm::translate(model, arrToVec3(tank_transforms + 10 * i));
		model = glm::scale(model, arrToVec3(tank_transforms + 10 * i + 7));

		for (int j = 0; j < 36; ++j) {
			glm::vec4 point = glm::vec4(vertices[j * 6], vertices[j * 6 + 1], vertices[j * 6 + 2], 1.0f);
			point = model * point;
			if (abs(point.x) > 10 || abs(point.z) > 10) return true;
		}
	}
	return false;
}
