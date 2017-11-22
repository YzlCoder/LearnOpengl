#pragma once
// GLFW
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>


#include "SpriteRender.h"


enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

class Game
{
public:
	// ��Ϸ״̬
	GameState  State;
	GLboolean  Keys[1024];
	GLuint     Width, Height;
	// ���캯��/��������
	Game(GLuint width, GLuint height);
	~Game();
	// ��ʼ����Ϸ״̬���������е���ɫ��/����/�ؿ���
	void Init();
	// ��Ϸѭ��
	void ProcessInput(GLfloat dt);
	void Update(GLfloat dt);
	void Render();
private:
	SpriteRenderer  *Renderer;
};