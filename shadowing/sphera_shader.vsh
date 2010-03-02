vs_1_1
dcl_position v0
dcl_color v1
dcl_normal v3

def c100, 0.0, 0.0, 0.0, 0.0
def c111, 1.0, 1.0, 1.0, 1.0 
;;;;;;;;;;;CONSTANTS;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;c0	- final radius
;c1	- time
;c2	- world matrix
;c12 - direct light direction
;c13 - direct light color
;c14 - direct light coefficient
;c15 - ambient light color
;c16 - point light color
;c17 - point light position
;c18 - attenuation
;c19 - specular light coefficient
;c20 - glare
;c21 - eye position
;c22 - spot light position
;c23 - spot light color
;c24 - cpot light direction
;c25 - 1/(INNER_CONE - OUTER_CONE) 
;c26 - OUTER_CONE/(INNER_CONE - OUTER_CONE)
;c27 - shift matrix

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;MORPHING;;;;;;;;;;;;;;;;;;;;;;;;
;Vertices
dp3 r0, v0, v0
rsq r0, r0
mov r2, c111            ; r2 = 1.0f	
mad r1, r0, c0, -r2		; r1 = ((final_rafius/r0) - 1)
mad r1, r1, c1, r2		; r1 = 1+t*((final_rafius/r0) - 1)
mul r2.xyz, v0.xyz, r1.x
m4x4 r1, r2, c27		; r1 - vertex!

;Normal
mul r3, v0, r0.x         
mov r3.w, c100.w         
add r4, r3, -v3         ; r4 = (r/|r| - n)
mad r4, r4, c1.x, v3    ; r4 = n + (r/|r| - n)*t
dp3 r0, r4, r4          ; r0 = |r4|**2
rsq r7, r0              ; r7 = 1/|r4|
mul r4, r4, r7.x        ; normalized r4
mov r10, r4

;;;;;;;;;;;;;;;;;;;LIGHTING;;;;;;;;;;;;;;;;;;;;;;;;;
;Vector from vertex to camera
add r9, c21, -r1        ; r9 = eye - vertex
dp3 r0, r9, r9          ; 
rsq r7, r0              ; normalize r7
mul r9, r9, r7.x        ; r9 - normalize vector from vertex to camera!
;--------------------------------------------------;
;Directional light
dp3 r5, c12, r10        ; r5 = (n,l)

;Diffuse directional light
mul r4, c13, r5.x       ; r4 = ld*(n,l)
mul r4, r4, c14.x       ; r4 = pd*ld*(n,l) 
max r6, r4, c100        ; checking negative color, r6+= diffuse directional light!

;Specular directional light
  add r2, r5, r5
  mad r2, r2, r10, -c12
dp3 r8, r2, r9          ; r8 = cos(phi)
mov r8.w, c20.x
lit r8, r8              ; r8.z = cos(phi)^f
mul r4, c13, r8.z       ; r4 = ld*cos(phi)^f
  mad r6, r4, c19.x, r6
;--------------------------------------------------;
;Point light

;direction vector
add r11, c17, -r1       ; r11 = point - vertex
dp3 r2, r11, r11        
rsq r7, r2              ; normalize r7
mul r11, r11, r7.x      ; r11 - normalize directional vector!
dp3 r5, r11, r10        ; r5 = cos(theta)

;attenuation
dst r2, r2, r7          ; r2 = (1, d, d**2, 1/d)
dp3 r0, r2, c18         ; r0 = (a + b*d + c*d**2)
rcp r0, r0              ; r0 = attenuation! 

;Point diffuse light
mul r4, c16, r5.x       ; r4 = ld*cos(theta)
mul r4, r4, r0.x        ; r4 = attenuation*pd*ld*cos(theta)
max r4, r4, c100        ; checking negative color
  mad r6, r4, c14.x, r6
;Point specular color
 add r2, r5, r5
 mad r2, r2, r10, -r11
dp3 r8, r2, r9          ; r8 = cos(phi)
mov r8.w, c20.x
lit r8, r8              ; r8.z = cos(phi)^f
 
mul r4, c16, r8.z       ; r4 = ld*cos(phi)^f
mul r4, r4, r0.x        ; r4 = attenuation * coef(specular)*ld*cos(phi)^f
  mad r6, r4, c19.x, r6
;--------------------------------------------------; 
;Spot light
;direction vector
add r11, c22, -r1       ; r11 = spot_light - vertex
dp3 r2, r11, r11        ; 
rsq r7, r2              ; normalize r11
mul r11, r11, r7.x      ; r11 - normalized direction vector!
dp3 r5, r11, r10        ; r5 = cos(theta)
;attenuation
dst r2, r2, r7          ; r2 = (1, d, d**2, 1/d)
dp3 r0, r2, c18         ; r0 = (a + b*d + c*d**2)
rcp r0, r0              ; r0 = attenuation!

dp3 r3, r11, c24        ; r3 = x = cos(l,d)
mov r7, c25             ; r7 = 1/(IN - OUT)
mad r2, r3, r7, -c26    ; r2 = (x - OUT)/(IN - OUT) interpolation
max r2, r2, c100        ; r2 = 0 if r2 < 0
min r2, r2, c111      
mul r0, r0, r2.x       

;Spot diffuse light
mul r4, c23, r5.x       ; r4 = ld*cos(theta)
mul r4, r4, r0.x        ; r4 = attenuation*pd*ld*cos(theta)
max r4, r4, c100        ; checking negative color
  mad r6, r4, c14.x, r6
;Spot specular light
; calculating r:
  add r2, r5, r5
  mad r2, r2, r10, -r11
dp3 r8, r2, r9          ; r8 = cos(phi)
mov r8.w, c20.x
lit r8, r8              ; r8.z = cos(phi)^f
 
mul r4, c23, r8.z       ; r4 = ld*cos(phi)^f
mul r4, r4, r0.x        ; r4 = attenuation *pd*ld*cos(phi)^f
 mad r6, r4, c19.x, r6
; Ambient light
add r6, r6, c15         ; r6 += ambient
 
;--------------------------------------------------;
m4x4 oPos, r1, c2
mov oD0, v1
mul oD0, v1, r6
