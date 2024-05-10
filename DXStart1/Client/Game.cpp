#include "pch.h"
#include "Game.h"
#include "Engine.h"

shared_ptr<Mesh> mesh = make_shared<Mesh>();
shared_ptr<Shader> shader = make_shared<Shader>();

void Game::Init(const WindowInfo& wInfo)
{
	GEngine->Init(wInfo);

	// 삼각형 띄우기 테스트 코드
	{
		vector<Vertex> vec(4);

		vec[0].pos = Vec3(-0.5f, -0.5f, 0.5f);
		vec[0].color = Vec4(1.f, 0.f, 0.f, 1.f);

		vec[1].pos = Vec3(-0.5f, 0.5f, 0.5f);
		vec[1].color = Vec4(0.f, 1.f, 0.f, 1.f);

		vec[2].pos = Vec3(0.5f, -0.5f, 0.5f);
		vec[2].color = Vec4(0.f, 0.f, 1.f, 1.f);
		
		vec[3].pos = Vec3(0.5f, 0.5f, 0.5f);
		vec[3].color = Vec4(1.f, 0.f, 0.f, 1.f);

		mesh->Init(vec, {0, 1, 2, 2, 1, 3});
	}

	shader->Init(L"..\\Resources\\Shader\\default.hlsli");

	GEngine->GetCmdQueue()->WaitSync();
}

void Game::Update()
{
	// GEngine->Render();

	
	GEngine->RenderBegin();

	shader->Update();

	/*mesh->Render();*/

	{
		Transform t;
		t.offset = Vec4(0.0f, 0.0f, 0.0f, 0.0f);
		mesh->Render();
	}

	GEngine->RenderEnd();
}
