#include "pch.h"
#include "Game.h"
#include "Engine.h"
#include "Mesh.h"
#include "Shader.h"

shared_ptr<Mesh> mesh = make_shared<Mesh>();
shared_ptr<Shader> shader = make_shared<Shader>();

void Game::Init(const WindowInfo& wInfo)
{
	GEngine->Init(wInfo);

	vector<Vertex> verticies(3);

	verticies[0].pos = Vec3(-1.0f, -0.707f, 0);
	verticies[0].color = Vec4(1, 1, 1, 1);

	verticies[1].pos = Vec3(0, 0.707f, 0);
	verticies[1].color = Vec4(1, 1, 1, 1);

	verticies[2].pos = Vec3(1.0f, -0.707f, 0);
	verticies[2].color = Vec4(1, 1, 1, 1);

	mesh->Init(verticies);
	shader->Init(TEXT("..\\Resources\\Shader\\default.hlsli"));

	GEngine->GetCmdQueue()->WaitSync();
}

void Game::Update()
{
	//GEngine->Render();

	GEngine->RenderBegin();

	shader->Update();
	mesh->Render();

	GEngine->RenderEnd();
}
