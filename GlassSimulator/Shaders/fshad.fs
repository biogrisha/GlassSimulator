#version 330 core
out vec4 FragColor;

flat in vec4 col;
in vec4 pos;
in vec4 norm;
in vec4 fragPos;

vec4 light = vec4(0.577350f,0.57735026f,0.577350f,0);

uniform int mod = 0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec2 resolution;
uniform vec3 viewPos;


float SquareIntegrl(float a,float b,float p, float p2,float x)
{
	return (x*((b*b + 2*a*b + a*a)*p*p*x*x + (-3*b*b - 3*a*b)*p*x+3*b*b))/3;
}

float SinIntergrl(float a,float b,float p, float p2,float x)
{
	return p2*(-(cos(a*(1-p*x)+b*p*x))/(b*p-a*p));

}
vec3 calcIntegral(vec3 val1,vec3 val2Temp)
{
	vec4 viewDir = normalize(vec4(viewPos.xyz,0) - fragPos); 
	vec3 dist= normalize(val2Temp-val1);
	float distTemp = length(val2Temp-val1);
	float coef =2;
	float distCoef = min(distTemp,coef);

	vec3 val2 = val1+(dist*distCoef);
	float x = 0;
	float y = 0;
	float z = 0;
	float val1res = 0;
	float r = 0;
	float g = 0.7;
	float b = 1;
	//for(float i = 0.3;i < 0.7; i+=0.05)
	//{
	//	x = (1-i)*val1.x + i*val2.x;
	//	y = (1-i)*val1.y + i*val2.y;
	//	z = (1-i)*val1.z + i*val2.z;
	//
	//	vec3 posIn = vec3(sin((x)*100),cos((x)*100),cos((x)*100));
	//	vec4 reflectDir = normalize(reflect(-light, vec4(posIn,0)));
	//	r= r*0.9 + pow(max(dot(reflectDir, viewDir),0),2)*0.1;
	//
	//}

	r+= pow((1-distTemp),3)/2;
	return vec3(max(r,0.1),max(r,0.1),max(r,0.1));
}

void main()
{    
	if(mod == 0)
	{
		FragColor = col;// + vec4(col.xyz,1);// + vec4(col.xyz,pos.z);

	}
	else if(mod == 1)
	{
		vec2 ss = vec2(gl_FragCoord.x / resolution.x, gl_FragCoord.y/resolution.y);

		if((float(texture(texture1, ss.xy).a)) >= pos.z)
		{
			discard;
		}else
		{
		
		vec3 funcColor = calcIntegral(texture(texture1, ss.xy).xyz,fragPos.xyz);
		FragColor = vec4(funcColor.xyz,1);

		}

	}
	else if(mod == 3)
	{
		FragColor = vec4(fragPos.xyz,pos.z);

	}
	else if(mod == 2)
	{
		vec4 viewDir = normalize(vec4(viewPos.xyz,0) - fragPos); 
		vec4 reflectDir = normalize(reflect(-light, norm));
		//float spec = pow((cos((  max(dot(viewDir, reflectDir),0)+1)*3.1415)+1)/2,4);
		float glow = pow(max(dot(viewDir, reflectDir), 0.0), 32);
		float spec = min(pow((cos(max(dot(viewDir, reflectDir),0.8)+2.30)+1)/0.01,4),0.3);
		// + vec4(col.xyz,1);// + vec4(col.xyz,pos.z);
		float shadow = dot(light, norm);
		vec2 ss = vec2(gl_FragCoord.x / resolution.x, gl_FragCoord.y/resolution.y);
		float transpar = texture(texture1, ss.xy).x;
		FragColor = vec4(texture(texture1, ss.xy).xyz,1)+(col/7)+vec4(1,1,1,0)*glow;

	}
}