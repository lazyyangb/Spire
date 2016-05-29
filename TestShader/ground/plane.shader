using "../EnginePipeline.shader";

shader Plane
{
    using Header;
    
	@perInstanceUniform sampler2D baseMap;
	@perInstanceUniform sampler2D baseNormalMap;
	inline vec2 uvCoord = vert_uv * 3.0;
	
	vec3 Normal
	{
		return normalize(texture(baseNormalMap, vec2(uvCoord.y, uvCoord.x)).xyz*2.0-vec3(1.0,1.0,1.0));
	}
	
	float Roughness = 0.5; 
    float Metallic = 0.1;
	float Specular = 1.0;
    [RGB8]
	vec3 Albedo
	{
		return texture(baseMap, uvCoord).xyz * 2.0;
	}
    
    using Footer;
}