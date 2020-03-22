#pragma once

class ModelAnimator
{
public:
	ModelAnimator(Shader* shader);
	~ModelAnimator();

	void Update();
	void Render();
	void Render2();
	void Render3();
	void Render4();

public:
	void ReadMaterial(wstring file);
	void ReadMesh(wstring file);
	void ReadClip(wstring file);

	void PlayClip(UINT instance, UINT clip, float speed = 1.0f, float takeTime = 1.0f);

	Model* GetModel() { return model; }

	void Pass(UINT pass);

	int weapon_num = 0;

	void SetWeaponNum(int weapon_num) { weapon_num %= 4; this->weapon_num = weapon_num; }

	bool GetStopAnim(int animnum) { return stopanim[animnum]; }
	void SetStopAnim(int animnum, const bool& is) { this->stopanim[animnum] = is; }

	void SetSrv(int weapon_num);

	Transform* AddTransform();
	Transform* GetTransform(UINT index) { return transforms[index]; }
	void UpdateTransforms();

	Matrix GetAttachTransform(UINT index);

	int GetTime() { return runningtime; }
	int GetTime(int mon_num) { return monrunningtime[mon_num]; }
	void SetTime(int a) { this->runningtime = a; }

	void CreateComputeDesc();
private:
	void CreateTexture(int temp);
	void CreateClipTransform(UINT index);

private:
	struct ClipTransform
	{
		Matrix** Transform;

		ClipTransform()
		{
			Transform = new Matrix*[MAX_MODEL_KEYFRAMES];

			for (UINT i = 0; i < MAX_MODEL_KEYFRAMES; i++)
				Transform[i] = new Matrix[MAX_MODEL_TRANSFORMS];
		}

		~ClipTransform()
		{
			for (UINT i = 0; i < MAX_MODEL_KEYFRAMES; i++)
				SafeDeleteArray(Transform[i]);

			SafeDeleteArray(Transform);
		}
	};
	ClipTransform* clipTransforms = NULL;

	ID3D11Texture2D* texture = NULL;
	ID3D11ShaderResourceView* srv = NULL;

	ID3D11Texture2D* sword_texture_spine = NULL;
	ID3D11ShaderResourceView* sword_srv_spine = NULL;

	ID3D11Texture2D* sword_texture = NULL;
	ID3D11ShaderResourceView* sword_srv = NULL;
	
	ID3D11Texture2D* bow_texture = NULL;
	ID3D11ShaderResourceView* bow_srv = NULL;

private:
	struct KeyframeDesc
	{
		int Clip = 0;

		UINT CurrFrame = 0;
		UINT NextFrame = 0;

		float Time = 0.0f;
		float RunningTime = 0.0f;

		float Speed = 1.0f;

		Vector2 Padding;
	}; //keyframeDesc;

	struct TweenDesc
	{
		float TakeTime = 1.0f;
		float TweenTime = 0.0f;
		float RunningTime = 0.0f;
		float Padding;

		KeyframeDesc Curr;
		KeyframeDesc Next;

		TweenDesc()
		{
			Curr.Clip = 0;
			Next.Clip = -1;
		}
	} tweenDesc[MAX_MODEL_INSTANCE];

	ConstantBuffer* frameBuffer;
	ID3DX11EffectConstantBuffer* sFrameBuffer;

private:
	struct CS_InputDesc
	{
		Matrix Bone;
	};

	struct CS_OutputDesc
	{
		Matrix Result;
	};

	struct AttachDesc
	{
		UINT AttachBoneIndex = 35;
		float Padding[3];
	} attachDesc;

private:
	Shader* shader;
	Model* model;
	
	vector<Transform *> transforms;
	Matrix worlds[MAX_MODEL_INSTANCE];

	VertexBuffer* instanceBuffer;

	int runningtime = 0;

	int monrunningtime[10];
	bool stopanim[10];
private:
	Shader* computeShader;
	StructuredBuffer* computeBuffer = NULL;

	CS_InputDesc* csInput = NULL;
	CS_OutputDesc* csOutput = NULL;
	
	ConstantBuffer* computeAttachBuffer;
	ID3DX11EffectConstantBuffer* sComputeAttachBuffer;
	ID3DX11EffectConstantBuffer* sComputeFrameBuffer;

	ID3DX11EffectShaderResourceVariable* sSrv;
	ID3DX11EffectUnorderedAccessViewVariable* sUav;
};