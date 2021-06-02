#include "colors.inc"
#include "textures.inc"
#include "transforms.inc"

#if(clock_on =1)
#declare Clock = clock;
#else
#declare Clock = 50/200;
#end

#declare TargetPos = <-0.33,3.35,0>;

#macro Step(A,B)
  max(0,min(1,(Clock-A)/(B-A)))
#end
#macro SmoothStep(A,B)
  #local T = Step(A,B);
  (T*T*(3-2*T))
#end

#declare CameraPos = TargetPos + 
      <2-5*SmoothStep(0.1,0.3)+8.0*SmoothStep(0.5,0.7)-4*SmoothStep(0.7,0.95),4,-4>  
    * exp(-3.0*(SmoothStep(0.1,0.6)-SmoothStep(0.6,0.9))) ;


camera {
  location CameraPos
  right x*image_width/image_height
  look_at
    0 + <0,2,0>*SmoothStep(0.0,0.1) + <-0.33,1.35,0>*SmoothStep(0.1,0.2)
      + <0.33,-1.0,-0.1>*SmoothStep(0.8,1.0)
  angle 55
} 

#declare MaxLevel = 8*Step(0,0.8) - 2*Step(0.85,1.0);
#declare MaxLevelF = MaxLevel-floor(MaxLevel);

/*
camera {
  location <0,-0.8,-10>
  look_at -y*0.8
  angle 40
}
*/

// Create an infinite sphere around scene and allow any pigment on it
sky_sphere {
  pigment {
    gradient y
    color_map { [0.0 color rgb <0.7,0.7,1.0>] [1.0 color blue 0.5] }
  }
}



light_source{
  <0.5,10,-4> White
  area_light 0.1*x,0.1*y,4,4
}



// light_source{<-3,10,-3> White}
// light_source{TargetPos White shadowless}

light_source { #local T = 0.1; CameraPos*(1-T)+TargetPos*T White shadowless }
plane {y,-.9 pigment {White}}

#declare Sqrt_3 = sqrt(3);

#macro Crv(C,T)
  #local IT = 1-T;
  #local P01 = IT*C[0]+T*C[1];
  #local P12 = IT*C[1]+T*C[2];
  #local P23 = IT*C[2]+T*C[3];
  #local P012 = IT*P01+T*P12;
  #local P123 = IT*P12+T*P23;
  (IT*P012+T*P123)  
#end

#declare H_A = 3.5;
#declare H_C = 1.95+0.10;
#declare H_S = 0.45;


#declare H_F = -ln(H_S);
#declare H_B = H_A*H_S*0.75;
#declare H_A0 = H_C/sqrt(3);

#macro MakeCrv(P,E0,E1,S)
  #local C = array[4];
  #local C[0] = P;
  #local C[1] = P + S*(E0*H_A0 + E1*H_C);
  #local C[2] = P + S*(E0*H_A + E1*H_C);
  #local C[3] = P + S*(E0*H_A + E1*H_B);
  C  
#end




#macro Horn(P,E0,E1,E2,S,L)
  #local C0 = MakeCrv(P,E0,E1,S);
  #local C1 = MakeCrv(P,E0,-E1,S);
  
  sphere {P, 0.5*S, -1 }
  #local TMax = 1;
  #if(L+1>MaxLevel)
    #local TMax = MaxLevelF;
  #end
  
  #local N = 2 + floor(TMax * 40 + L*4); 
  #local i = 1;
  #while(i<N)
    #local T = TMax * i/(N-1); 
    #local R = 0.7 * S * exp(-T*H_F) * (1-0.5*4*T*(1-T));
    sphere {Crv(C0,T), R 1 }
    sphere {Crv(C1,T), R 1 }
    #local i=i+1;
  #end
  #if(L+1<MaxLevel)
    #local S1 = S * H_S;
    Horn(C0[3],-E1,E0,E2,S1,L+1)
    Horn(C1[3], E1,E2,E0,S1,L+1)
  #end  
#end

#macro HornPos(P,E0,E1,E2,S,L)
  #local Ret = P;
  #if(L>0)
    #local S1 = S * H_S;
    #local Ret = HornPos(P + S*(E0*H_A + E1*H_B),-E1,E0,E2,S1,L-1);
  #else
    #local Ret = P + S*E0*H_A;
  #end  
  Ret
#end

#declare Root_P0 = <0,0,0>;
#declare Root_E0 = <0,1,0>;
#declare Root_E1 = <1,0,0>;
#declare Root_E2 = <0,0,1>;
#declare Root_S = 1;

union {

  blob {
      
    Horn(Root_P0,Root_E0,Root_E1,Root_E2,Root_S,0)
    sphere {0,1.5 2.5}
    texture {
      pigment {Orange}
      finish {
        ambient 0.6
        diffuse 0.4
        specular 0.8
        roughness 0.003      
      } 
     normal {bumps 0.25 scale 0.012 }
    }
  }
  

}



//#declare TargetPos = HornPos(Root_P0,Root_E0,Root_E1,Root_E2,Root_S,6);
//#debug concat(str(TargetPos.x,5,2),",",str(TargetPos.y,5,2),",",str(TargetPos.z,5,2),"\n")

