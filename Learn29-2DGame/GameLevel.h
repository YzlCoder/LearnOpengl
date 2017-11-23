#pragma once
#include "define.h"
#include "GameObject.h"

class GameLevel
{
public:
	std::vector<GameObject> Bricks;

	GameLevel() { }
	// ���ļ��м��عؿ�
	void Load(const GLchar *file, GLuint levelWidth, GLuint levelHeight);
	// ��Ⱦ�ؿ�
	void Draw(SpriteRenderer &renderer);
	// ���һ���ؿ��Ƿ������ (���зǼ�Ӳ�Ĵ�ש�����ݻ�)
	GLboolean IsCompleted();
private:
	// ��ש�����ݳ�ʼ���ؿ�
	void init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight);
};