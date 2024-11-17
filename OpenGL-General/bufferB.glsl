#version 460 core



layout(binding = 0, rgba32f) uniform image2D accumBuffer;

 precision highp float;


 in vec2 texCoord;

///uniform
uniform int frame;
uniform float time;
uniform vec2 resolution;
uniform vec4 mouse;
uniform sampler2D tex0;
uniform vec3 cameraPosition;


uniform mat4 view_Matrix;
uniform mat4 projection_Matrix;

 #define f 0.00004
 #define LIMIT 1000
 #define eps 0.0001
   const uint k = 1103515245U;  // for hash functions 
   float aspect_ratio = resolution.x/resolution.y;

 vec3 null;

 vec3 hash33( uvec3 x )
{
    x = ((x>>8U)^x.yzx)*k;
    x = ((x>>8U)^x.yzx)*k;
    x = ((x>>8U)^x.yzx)*k;
    
    return vec3(x)*(1.0/float(0xffffffffU));
}


vec3 jitter = hash33(uvec3( texCoord * 1024 * vec2(aspect_ratio,1.0) , frame)) - 0.5f;
 struct global {

  vec3 breaknormal;
  vec3 emptyspcaeNormal;
  float limit;

  };


      struct varriables {
    vec3 color;
    vec3 normal;
  };
  varriables gl;

  struct r {
  vec3 Direction ;
  vec3 Origin ;
   float Distance;
  };

  r ray;


  mat4 invProject_m = inverse(projection_Matrix);
  mat4 invView_m = inverse(view_Matrix);

 

  ///Uniform function ///
 
  vec3 makeHitpoint( vec3 rayDirection , vec3 rayOrgin , float Distance){
    return (rayDirection*Distance) + rayOrgin ;
  }

vec3 aces(vec3 x) {
  const float a = 2.51;
  const float b = 0.03;
  const float c = 2.43;
  const float d = 0.59;
  const float e = 0.14;
  return clamp((x * (a * x + b)) / (x * (c * x + d) + e), 0.0, 1.0);
}

  //-----------------------------------------------------
// Intersection functions (by iq)
//-----------------------------------------------------

vec3 nSphere( in vec3 pos, in vec4 sph ) {
    return (pos-sph.xyz)/sph.w;
}

float iSphere( in vec3 ro, in vec3 rd, in vec4 sph ) {
    vec3 oc = ro - sph.xyz;
    float b = dot(oc, rd);
    float c = dot(oc, oc) - sph.w * sph.w;
    float h = b * b - c;
    if (h < 0.0) return -1.0;

	float s = sqrt(h);
	float t1 = -b - s;
	float t2 = -b + s;
	
	return t1 < 0.0 ? t2 : t1;
}

//Data Draw//

    vec4 sphere1_pos = vec4( 0.0,0.0, -1.0, 0.5f) ;       vec3 col1 = vec3(1.0f,0.3f,0.4f);
     vec4 sphere2_pos = vec4( -1.5,0.166, -1.7, 0.666f) ;   vec3 col2 = vec3(0.3f,0.3f,1.0f);
      vec4 sphere3_pos = vec4( 1.5,1.0, -1.7, 0.8f) ;       vec3 col3 = vec3(0.3f,1.0f,0.4f);

      vec4 sphereG_pos = vec4( 0.0,-3000.0-0.5f, -1.0, 3000.0f) ;  vec3 groundSpColor = vec3(0.7f); 


      

 vec2 intersectSceneTest( vec3 rayDirection ,vec3 rayOrigin, bool Color ,inout vec3 normal ) {


 //note : color is only for albedo reflection and colored shadows ;
  int i ;  // a empty integer z 
 	vec2 res = vec2( 1e20, -1.0 );
    float t;

    ///virtual buffer /// 
      
      //positions  and radius                                        //Color



      /// draw scene ///
t = iSphere( rayOrigin, rayDirection,sphere1_pos ); if( t>eps && t<res.x ){ res = vec2( t, 1. ); normal = nSphere( rayOrigin+t* rayDirection, sphere1_pos ); }
t = iSphere( rayOrigin, rayDirection,sphere2_pos); if( t>eps && t<res.x ) { res = vec2( t, 2. ); normal = nSphere( rayOrigin+t* rayDirection,  sphere2_pos); }
t = iSphere( rayOrigin, rayDirection,sphere3_pos); if( t>eps && t<res.x ) { res = vec2( t, 3. ); normal = nSphere( rayOrigin+t* rayDirection,  sphere3_pos); }

///ground floor (a large sphere)

t = iSphere( rayOrigin, rayDirection,sphereG_pos ); if( t>eps && t<res.x ) { res = vec2( t, 32. ); normal = nSphere( rayOrigin+t* rayDirection, sphereG_pos ); }

if ( Color == true ) {
  if (res.y == 1.0) { gl.color = col1 ;}
  if (res.y == 2.0) { gl.color = col2 ;}
  if (res.y == 3.0) { gl.color = col3 ;}
  if (res.y == 32.0) { gl.color = groundSpColor ;}
   }
  ray.Distance = res.x;
    return res;					  
}

///Color index oriantation/// 



void drawPixelShade(vec3 normal , out float shade) {
   
    vec4 point_light_color = vec4(1.0f , 1.0f , 1.0f , 6.0f);
    vec3 pointLight_position = vec3(0.3,3.3,0.2);
       pointLight_position += jitter;
       pointLight_position = pointLight_position - makeHitpoint(ray.Direction , ray.Origin + gl.normal * 0.0006f , ray.Distance) ;
    vec3 lightVector = normalize(pointLight_position);
    float relativepoint = length(pointLight_position);

    float shadows = 1.0f; 

    if (true) {
    vec3 hitpoint = makeHitpoint(ray.Direction , ray.Origin , ray.Distance);
      hitpoint += gl.normal * 0.0006f;
    vec2 shadowRay = intersectSceneTest(lightVector , hitpoint, false , null );
       
        bool shadow = (shadowRay.x >= relativepoint); 
        
        shadows = float(shadow);
}


       shade = max( dot(normal,lightVector) , 0.0f) / (relativepoint * relativepoint); ;
       shade *= (shadows * point_light_color.a);
}

//General Buffer// 

out vec4 FragColor;
void main()
{
vec3 tcol;
int i;   
float tmap;
 

  

  vec2 st = texCoord;
   st = st*2.0f - 1.0f;  // -1 .... 1 //
 // st.x *= aspect_ratio;
     
  

     ray.Origin = cameraPosition;
vec4 target = invProject_m * vec4(st.xy,1,1);
 ray.Direction = vec3(invView_m * vec4( normalize(target.xyz/target.w) , 0 ));
   
  
  vec2 res = intersectSceneTest(ray.Direction,ray.Origin, true ,gl.normal);

     drawPixelShade(gl.normal , tmap);
       
     tcol = tmap*gl.color;

     tcol = aces(tcol);
     tcol = pow(tcol,vec3(1.0f/2.2f));

     imageStore(accumBuffer,ivec2(texCoord),vec4(tcol,1.0f));

   FragColor = vec4(vec3(tcol),1.0f);
}