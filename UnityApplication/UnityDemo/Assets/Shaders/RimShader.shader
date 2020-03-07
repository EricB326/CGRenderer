Shader "UnityDemo/RimShader"
{
	Properties
	{
		_Colour("Colour", Color) = (1, 1, 1, 1)
		_MainTex("Albedo", 2D) = "white" {}

		_RimColour("Rim Colour", Color) = (1, 1, 1, 1)
		_RimPower("Rim Power", Range(0.5, 8.0)) = 3.0
	}

	SubShader
	{
		CGPROGRAM
			#pragma surface surf Standard fullforwardshadows

			fixed4 _Colour;
			sampler2D _MainTex;
			fixed4 _RimColour;
			fixed _RimPower;

			struct Input
			{
				float2 uv_MainTex;
				float3 viewDir;
			};
			
			void surf(Input IN, inout SurfaceOutputStandard o)
			{
				fixed4 c = tex2D(_MainTex, IN.uv_MainTex) * _Colour;
				o.Albedo = c.rgb;
				o.Alpha = c.a;

				half rim = 1.0 - saturate(dot(normalize(IN.viewDir), o.Normal));
				o.Emission = _RimColour.rgb * pow(rim, _RimPower);
			}

		ENDCG
	}

	FallBack "Diffuse"
}