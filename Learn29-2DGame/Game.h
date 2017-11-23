#pragma once
#include "define.h"
#include "GameLevel.h"
#include "SpriteRender.h"
#include "BallObject.h"
#include <tuple>
#include "Particle.h"
#include "PostProcessor.h"
#include "PowerUp.h"
#include <irrklang/irrKlang.h>
using namespace irrklang;

enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

// Represents the four possible (collision) directions
enum Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT
};
// Defines a Collision typedef that represents collision data
typedef std::tuple<GLboolean, Direction, glm::vec2> Collision; // <collision?, what direction?, difference vector center - closest point>

// ��ʼ������Ĵ�С
const glm::vec2 PLAYER_SIZE(100, 20);
// ��ʼ�����������
const GLfloat PLAYER_VELOCITY(500.0f);

// ��ʼ������ٶ�
const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
// ��İ뾶
const GLfloat BALL_RADIUS = 12.5f;


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
	void DoCollisions();
	// Reset
	void ResetLevel();
	void ResetPlayer();
private:
	GLboolean CheckCollision(GameObject &one, GameObject &two); // AABB - AABB collision
	Collision CheckCollision(BallObject &one, GameObject &two); // AABB - Circle collision
	Direction VectorDirection(glm::vec2 target);
	GLboolean ShouldSpawn(GLuint chance);
	void SpawnPowerUps(GameObject &block);
	void UpdatePowerUps(GLfloat dt);
	void ActivatePowerUp(PowerUp &powerUp);

	std::vector<GameLevel> Levels;
	GLuint                 Level;
	SpriteRenderer  *Renderer;
	GameObject *Player;
	BallObject *Ball;
	ParticleGenerator *Particles;
	PostProcessor     *Effects;
	GLfloat            ShakeTime;
	std::vector<PowerUp>  PowerUps;
	ISoundEngine *SoundEngine;

};