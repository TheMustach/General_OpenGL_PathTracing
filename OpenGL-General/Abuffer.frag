#version 460 core


 precision highp float;
///uniform
uniform int frame;
uniform float time;
uniform vec2 resolution;
uniform vec3 mouse;
uniform sampler2D tex0;

#define f 0.00004
#define LIMIT 1000
 
  struct global {

  vec3 breaknormal;
  vec3 emptyspcaeNormal;
  float limit;
  };


     vec3 gHitpoint(vec3 rayDirection, vec3 rayOrigin, float gDistance ){
         return (rayDirection*gDistance)+rayOrigin;
     }     
        /// Vector to light equation   /// For point Lights
       vec3 vecToLight(vec3 hitpoint, vec3 lightPosition){
         vec3 lightVectorConst = lightPosition-hitpoint;
         return normalize(lightVectorConst);
      } 



int iSphere(vec3 rayDirection, vec3 rayOrigin, in vec3 vaPosition,in vec3 vaColor,
      float vaRadius,inout vec3 wNormal,inout float wDistance,inout vec3 wPosition, inout vec3 wColor
      ) {

   
        float dotPosition = max(dot((vaPosition-rayOrigin),rayDirection),0.0);  /// length ////   
           float bDistance = distance(vaPosition,gHitpoint(rayDirection,rayOrigin,dotPosition));
           float aIntersection = sqrt(pow(vaRadius,2.)-pow(bDistance,2.));
        float Distance = (dotPosition-aIntersection); 
          float rays_distance = distance(rayOrigin,vaPosition);

         bool rayHit = (rays_distance > (vaRadius+f)); 

        int relativeDistance = int(bDistance < vaRadius)*int(rayHit);

        vaColor = vaColor*vec3(relativeDistance);
          ///intersection bool/// 
        if(relativeDistance != 0) {
            Distance = Distance;
        }
        else {
          Distance = float(LIMIT);
        }
           wDistance = min(Distance,wDistance);
        ///Normal Struct/// 
        vec3 Normal = gHitpoint(rayDirection,rayOrigin,wDistance)-vaPosition;
         Normal = normalize(Normal);

           bool cDistance = (Distance > wDistance+f);
          if (Distance  >= wDistance ) {Normal = wNormal;
                                      wColor = wColor;
                                      wPosition = wPosition;       
                            }
          else {wNormal = Normal;
                wColor = vaColor;
                wPosition = vaPosition;
    }
    return 1;

     }


     struct ball1 {
       int index;
       vec3 color;
       vec3 postition ;
       float radius ;
       bool valid;
     };

     

     void recalldata() {

       ball1 ball01; ball01.valid = true ;

       if (ball01.valid == true) {
         ball01.index = 1;
         ball01.postition = vec3(0.2,0.3,0.8);
         ball01.radius = 0.5;
         ball01.color = vec3(1.0);
         }
     }



//General Buffer// 
in vec2 texCoord;
out vec4 FragColor;
void main()
{
   
  
  float aspect_ratio = resolution.x/resolution.y;

  vec2 st = texCoord;
  st.x *= aspect_ratio;

   vec3 image = texture(tex0 , texCoord).rgb;
    
   uvec3 p = uvec3(st*1000.0, frame) ;
   
  // vec3 c = hash33(p);

   FragColor = vec4(vec3(texCoord ,0.0), 1.0f);
}