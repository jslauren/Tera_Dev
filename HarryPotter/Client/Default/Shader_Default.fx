// 전역 변수
matrix	g_matWorld, g_matView, g_matProj;

float	g_fData = 10.0f;
float3	g_Light = { 1.f, -1.f, 0.f };

texture g_BaseTexture;

sampler BaseSampler = sampler_state
{
	Texture = g_BaseTexture;
};

// 셰이더로 그릴준비를한다.(begin()).

// 버퍼를 그린다.

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

// 반드시 필요한 기능을 구현.
// 추가적인 기능구현.

// 정점버퍼에 있었던 정점의 정보를 VS_IN구조체 안에 담아서  VS_MAIN함수의 인자로 전달한다.
VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT		Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	// 정점의 월드변환, 뷰변환, 투영행렬의 곱까지 수행한다.
	Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;
	Out.vLocalPos = vector(In.vPosition, 1.f);

	return Out;
};
// VS_OUT안에 정의되어있는 POSITION시멘틱에 해당하는 변수의 W값으로 XYZ를 나눈다.(원근투영)
// 뷰포트 변환.
// 래스터라이즈(픽셀이 생성된다.)

// 생성된 픽셀을 PS_IN에 담아서 PS_MAIN함수의 인자로 전달한다.

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


// 장치성능에 따른 셰이더 선택을 위해 존재.
technique Default_Device
{
	// 어떤 객체를 렌더링함에 있어 표현하고 싶은 렌더링효과의 셋트 기준으로 구분한다.
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

// 셰이더로 그리기작업을 마친다.

//_uint iNumPasses = 패스의 갯수를 얻어올수있다.;
//
//for (size_t i = 0; i < iNumPasses; i++)
//{
//	만욱이 렌더링.
//}
