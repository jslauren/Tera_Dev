// ���� ����
matrix	g_matWorld, g_matView, g_matProj;

float	g_fData = 10.0f;
float3	g_Light = { 1.f, -1.f, 0.f };

texture g_BaseTexture;

sampler BaseSampler = sampler_state
{
	Texture = g_BaseTexture;
};

// ���̴��� �׸��غ��Ѵ�.(begin()).

// ���۸� �׸���.

struct VS_IN
{
	float3		vPosition : POSITION;
	float3		vNormal	: NORMAL;
	float2		vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	float4		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vLocalPos : TEXCOORD1;
};

// �ݵ�� �ʿ��� ����� ����.
// �߰����� ��ɱ���.

// �������ۿ� �־��� ������ ������ VS_IN����ü �ȿ� ��Ƽ�  VS_MAIN�Լ��� ���ڷ� �����Ѵ�.
VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	// ������ ���庯ȯ, �亯ȯ, ��������� ������ �����Ѵ�.
	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	Out.vLocalPos = vector(In.vPosition, 1.f);

	return Out;
};
// VS_OUT�ȿ� ���ǵǾ��ִ� POSITION�ø�ƽ�� �ش��ϴ� ������ W������ XYZ�� ������.(��������)
// ����Ʈ ��ȯ.
// �����Ͷ�����(�ȼ��� �����ȴ�.)

// ������ �ȼ��� PS_IN�� ��Ƽ� PS_MAIN�Լ��� ���ڷ� �����Ѵ�.

struct PS_IN
{
	float4		vPosition : POSITION;
	float3		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	float4		vLocalPos : TEXCOORD1;
};

struct PS_OUT
{
	vector		vColor : COLOR;
};

PS_OUT PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	float3 vLight = normalize(mul(g_Light, -1));
	In.vNormal = normalize(In.vNormal);

	mul(Out.vColor, saturate(dot(vLight, In.vNormal)));

	return Out;
};


// ��ġ���ɿ� ���� ���̴� ������ ���� ����.
technique Default_Device
{
	// � ��ü�� �������Կ� �־� ǥ���ϰ� ���� ������ȿ���� ��Ʈ �������� �����Ѵ�.
	pass Default_State
	{
		CULLMODE = NONE;
		ALPHABLENDENABLE = TRUE;
		SRCBLEND = SRCALPHA;
		DESTBLEND = INVSRCALPHA;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile vs_3_0 PS_MAIN();
	}
};

// ���̴��� �׸����۾��� ��ģ��.

//_uint iNumPasses = �н��� ������ ���ü��ִ�.;
//
//for (size_t i = 0; i < iNumPasses; i++)
//{
//	������ ������.
//}
