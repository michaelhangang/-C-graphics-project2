#version 420 

in vec3 Normal;
in vec3 FragPos;
in vec4 fUVx2;
out vec4 FragColor;


uniform sampler2D texSamp2D_00;

uniform sampler2D texSamp2D_01;

uniform bool bUseTexture;
uniform bool isUseOneTexture;
uniform bool isUseTwoTexture;

uniform sampler2D texSamp2D_Alpha;
uniform bool bUseAlphaTexture;

uniform vec3 viewPos;
uniform vec4 ourColor;
uniform vec3 sphereColor;
uniform bool isLightSphere;
struct Light {
    vec3 position;  
   
    vec3 diffuse;
    float specular;
	vec3 atten;
	float isOn;
};
struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
 
    //vec3 ambient;
    vec3 diffuse;
    vec3 specular;  
	vec3 atten;
	float isOn;
};
const int NUMBEROFLIGHTS = 5;
uniform Light theLights[NUMBEROFLIGHTS];

const int NUMBEROFSPOTLIGHTS = 5;
uniform SpotLight theSpotLights[NUMBEROFSPOTLIGHTS];

vec3 CalcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir,vec4 color);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir,vec4 color);

void main()
{
    //properties
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 finalColor= vec3(0.0) ;
	vec4 color =ourColor;
	
	if(isLightSphere){
	    FragColor = vec4(sphereColor, 1.0);
		return;
	}
	
	if ( bUseTexture )
   {
	
	if(isUseOneTexture)
	color.xyz =texture( texSamp2D_00, fUVx2.xy ).rgb;
	else if(isUseTwoTexture)
		color.xyz =mix( texture( texSamp2D_00, fUVx2.xy ),texture( texSamp2D_01, fUVx2.xy ),0.1).rgb;  //0.1 second color

   }

   
	//Point light
	for(int index =0;index<NUMBEROFLIGHTS;index++){
	   
	    if(theLights[index].isOn ==0.0f)
		  {
			continue;
		   }

	   finalColor +=CalcPointLight(theLights[index],  norm,  FragPos,  viewDir, color) ;
	
	 }
	
	//Spot lights
    for(int index =0;index<NUMBEROFSPOTLIGHTS;index++){
	   
	    if(theSpotLights[index].isOn ==0.0f)
		  {
			continue;
		   }

	   finalColor +=CalcSpotLight(theSpotLights[index],  norm,  FragPos,  viewDir, color) ;
	
	 }
	 
	
 
 
	FragColor = vec4(finalColor,color.a);
	 if ( bUseAlphaTexture )
	{
		// Pass the 4th value (alpha) as the transparency value
		if( texture(texSamp2D_Alpha, fUVx2.xy ).p>0.9)
		FragColor.a=0;
		FragColor.a = texture(texSamp2D_Alpha, fUVx2.xy ).r*color.a;
		
	}
	
	
}

vec3 CalcPointLight(Light light, vec3 norm, vec3 fragPos, vec3 viewDir,vec4 color){
	   
    float ambientStrength = 0.05;
		 
	//diffuse 	
    vec3 lightDir = normalize(light.position- fragPos);
	float diff = max(dot(norm, lightDir), 0.0);

	//specular 
    vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	
	//attenua
	float distance    = length(light.position - FragPos);
    float attenuation = 1.0 / (light.atten.x + light.atten.y * distance +light.atten.z * (distance * distance));
	
	//set up ambient 
    vec3 ambient = ambientStrength * light.diffuse;
    vec3 diffuse = diff * light.diffuse;
    vec3 specular = light.specular * spec * light.diffuse;

	ambient  *= attenuation; 
    diffuse  *= attenuation;
    specular *= attenuation;

	
	return (ambient + diffuse + specular)* color.xyz;

}
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir,vec4 color){
     float ambientStrength = 0.0;
		 
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.atten.x + light.atten.y * distance +light.atten.z * (distance * distance));    
   
   // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = ambientStrength* light.diffuse;;
    vec3 diffuse = light.diffuse * diff ;
    vec3 specular = light.specular * spec *light.diffuse;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
   
    return (ambient + diffuse + specular)* color.xyz;
}

