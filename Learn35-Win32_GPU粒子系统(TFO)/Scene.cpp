#include "Scene.h"
#include "ggl.h"
#include "ShaderTool.h"
#include <cmath>
#include "utils.h"
#include "Timer.h"


struct VertexData
{
	union
	{
		struct
		{
			float pos[4];
			float mess[4];
		};
		float v[8];
	};
};

GLuint program;
GLuint emitter;//emitter pos
GLuint tfoNewParticle, tfoNewParticleBuffer;
GLuint translateMProgram;
GLint translateMPosLocation, translateMMLocation, translateMMessLocation;

GLuint updateParticleProgram = 0;
GLint updateParticleProgramPosLocation, updateParticleProgramMessLocation;
bool bEmitNewParticle = false;
GLuint updateParticleTFO[2];
GLuint updateParticleTFOBuffer[2];
int currentParticleTFO = 0;
int currentParticleTFOForDraw = 0;

GLuint queryObject;
GLint particleCount = -1;

glm::mat4 model;
glm::mat4 projection;

GLuint particleAlphaTexture;
GLint VLocation, PLocation, posLocation, messLocation, textureLocation;

Timer timer;
float lastPassTime = 0;

int frame = 0;
float frand();
float sfrand();
GLuint CreateTextureAlpha(int w, int h);

void EmitParticle()
{
	glEnable(GL_RASTERIZER_DISCARD);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, tfoNewParticle);
	glUseProgram(translateMProgram);
	glUniformMatrix4fv(translateMMLocation, 1, GL_FALSE, glm::value_ptr(model));

	glBeginTransformFeedback(GL_POINTS);
	glBindBuffer(GL_ARRAY_BUFFER, emitter);
	glEnableVertexAttribArray(translateMPosLocation);
	glVertexAttribPointer(translateMPosLocation, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), 0);
	glEnableVertexAttribArray(translateMMessLocation);
	glVertexAttribPointer(translateMMessLocation, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(sizeof(float) * 4));
	glDrawArrays(GL_POINTS, 0, 1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEndTransformFeedback();
	glUseProgram(0);
	glDisable(GL_RASTERIZER_DISCARD);
	bEmitNewParticle = true;
}


void UpdateParticle()
{
	//printf("current particle buffer tfo %d %d\n",currentParticleTFO, currentParticleTFOForDraw);
	int currentOldParticleBufferIndex = currentParticleTFOForDraw;
	if (particleCount == -1)
	{
		particleCount++;
	}
	else
	{
		glGetQueryObjectiv(queryObject, GL_QUERY_RESULT, &particleCount);
	}
	frame++;
	if (particleCount < 500 && frame % 200 == 0 )
	{
		EmitParticle();
	}
	glBindBuffer(GL_ARRAY_BUFFER, emitter);
	VertexData* vertexes = (VertexData*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_WRITE);
	vertexes[0].v[0] = sinf(timer.GetPassTime());
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glEnable(GL_RASTERIZER_DISCARD);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, updateParticleTFO[currentParticleTFO]);
	glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, queryObject);
	glUseProgram(updateParticleProgram);
	glBeginTransformFeedback(GL_POINTS);
	//new emitted particle update
	if (bEmitNewParticle)
	{
		bEmitNewParticle = false;
		//update particle : write new particle to some buffer via transform feedback technique
		glBindBuffer(GL_ARRAY_BUFFER, tfoNewParticleBuffer);
		glEnableVertexAttribArray(updateParticleProgramPosLocation);
		glVertexAttribPointer(updateParticleProgramPosLocation, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), 0);
		glEnableVertexAttribArray(updateParticleProgramMessLocation);
		glVertexAttribPointer(updateParticleProgramMessLocation, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(sizeof(float) * 4));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawTransformFeedback(GL_POINTS, tfoNewParticle);
	}
	//update old particle : write old particle to some buffer via transform feedback technique
	if (particleCount > 0)
	{
		glBindBuffer(GL_ARRAY_BUFFER, updateParticleTFOBuffer[currentOldParticleBufferIndex]);
		glEnableVertexAttribArray(updateParticleProgramPosLocation);
		glVertexAttribPointer(updateParticleProgramPosLocation, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), 0);
		glEnableVertexAttribArray(updateParticleProgramMessLocation);
		glVertexAttribPointer(updateParticleProgramMessLocation, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(sizeof(float) * 4));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawTransformFeedback(GL_POINTS, updateParticleTFO[currentOldParticleBufferIndex]);
	}

	glEndTransformFeedback();
	glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
	glUseProgram(0);
	glDisable(GL_RASTERIZER_DISCARD);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
	currentParticleTFOForDraw = currentParticleTFO;
	currentParticleTFO = (currentParticleTFO + 1) % 2;

	printf("%d\n",(int)(1/(timer.GetPassTime() - lastPassTime)));
	lastPassTime = timer.GetPassTime();
}




void Init()
{
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.0f));

	VertexData vertexes[1];
	vertexes[0].v[0] = 0.0f;
	vertexes[0].v[1] = -1.0f;
	vertexes[0].v[2] = 0.0f;
	vertexes[0].v[3] = 1.0f;

	vertexes[0].mess[0] = 0.0f;
	vertexes[0].mess[1] = 0.0f;
	vertexes[0].mess[2] = 0.0f;
	vertexes[0].mess[3] = 0.0f;

	emitter = CreateBufferObject(GL_ARRAY_BUFFER, sizeof(VertexData) * 1, GL_STATIC_DRAW, &vertexes);
	tfoNewParticleBuffer = CreateBufferObject(GL_ARRAY_BUFFER, sizeof(VertexData) * 1024, GL_STATIC_DRAW, nullptr);
	tfoNewParticle = CreateTransformFeedbackObject(tfoNewParticleBuffer);

	//local coordinate -> world coordinate : execute once
	const char *attribs[] = { "gl_Position" , "o_mess" };
	translateMProgram = CreateTFOProgram("tfo_translateM.vs", attribs, 2, GL_INTERLEAVED_ATTRIBS);
	translateMPosLocation = glGetAttribLocation(translateMProgram, "pos");
	translateMMessLocation = glGetAttribLocation(translateMProgram, "mess");
	translateMMLocation = glGetUniformLocation(translateMProgram, "M");

	EmitParticle();

	//above is success
	//world world -> screen coordinate : execute per frame
	program = CreateGPUProgram("tfo_translateScreen.vs", "tfo_translateScreen.fs", "tfo_translateScreen.gs");


	posLocation = glGetAttribLocation(program, "pos");
	VLocation = glGetUniformLocation(program, "V");
	PLocation = glGetUniformLocation(program, "P");
	messLocation = glGetAttribLocation(program, "mess");
	textureLocation = glGetUniformLocation(program, "U_MainTexture");

	particleAlphaTexture = CreateTextureAlpha(256, 256);

	//update particle shader
	updateParticleProgram = CreateTFOProgram("tfo_update_particle.vs", attribs, 2, GL_INTERLEAVED_ATTRIBS, "tfo_update_particle.gs");
	updateParticleProgramPosLocation = glGetAttribLocation(updateParticleProgram, "pos");
	updateParticleProgramMessLocation = glGetAttribLocation(updateParticleProgram, "mess");

	for (int i = 0; i < 2; ++i)
	{
		updateParticleTFOBuffer[i] = CreateBufferObject(GL_ARRAY_BUFFER, sizeof(VertexData) * 10240, GL_STATIC_DRAW, nullptr);
		updateParticleTFO[i] = CreateTransformFeedbackObject(updateParticleTFOBuffer[i]);
	}

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	float identity[] = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
	};
	glGenQueries(1, &queryObject);
	timer.Start();
}


void SetViewPortSize(float width, float height)
{
	projection = glm::perspective(45.0f, (float)width / (float)height, 0.1f, 1000.0f);
}
void Draw()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//update particle
	UpdateParticle();
	//draw particle
	glUseProgram(program);
	glUniformMatrix4fv(VLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4()));
	glUniformMatrix4fv(PLocation, 1, GL_FALSE, glm::value_ptr(projection));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, particleAlphaTexture);
	glUniform1i(textureLocation, 0);
	glBindBuffer(GL_ARRAY_BUFFER, updateParticleTFOBuffer[currentParticleTFOForDraw]);
	glEnableVertexAttribArray(posLocation);
	glVertexAttribPointer(posLocation, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), 0);
	glEnableVertexAttribArray(messLocation);
	glVertexAttribPointer(messLocation, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)(sizeof(float) * 4));
	
	glDrawTransformFeedback(GL_POINTS, updateParticleTFO[currentParticleTFOForDraw]);
//	glDrawArrays(GL_POINTS, 0, 1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glFlush();
	glUseProgram(0);

}

float frand()
{
	return rand() / (float)RAND_MAX;
}

float sfrand()
{
	return frand() * 2 - 1.0f;
}

GLuint CreateTextureAlpha(int w, int h)
{
	unsigned char*imageData = new unsigned char[w*h];
	float distanceMax = (float)w;
	for (int y = 0; y < h; ++y)
	{
		for (int x = 0; x < w; ++x)
		{
			float distance = sqrtf((x - w / 2.0f)*(x - w / 2.0f) + (y - h / 2.0f)*(y - h / 2.0f));
			float alpha = powf((distanceMax - distance) / distanceMax, 8.0f);
			alpha = alpha > 1.0f ? 1.0f : alpha;
			imageData[x + y*w] = alpha > 0.0f ? (unsigned char)(alpha*255.0f) : 0;
		}
	}
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, w, h, 0, GL_ALPHA, GL_UNSIGNED_BYTE, imageData);
	glBindTexture(GL_TEXTURE_2D, 0);
	delete imageData;
	return texture;
}