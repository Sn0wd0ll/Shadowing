vs_1_1
dcl_position v0
dcl_color v1
dcl_normal v3
 
;;;;;;;;Constants;;;;;;;;;;;;;;;;;;;
; c0 -  view matrix           
; c4 -  FINAL_RADIUS              
; c5 -  t                         
; c17-  point light position      
; c27-  position matrix    
; c31-  shadow projective matrix   
; c35-  shadow attenuation consts  
 
def c100, 0.0, 0.0, 0.0, 0.0
def c111, 1.0, 1.0, 1.0, 1.0 
 
;;;;;;;;;;;;;;;;;;;;;; Morphing ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;position
dp3 r0, v0, v0
rsq r0, r0              ; r0 = 1/|r|
 
mov r2, c111            ; r2 = 1.0f
mad r1, r0, c0, -r2    ; r1 = ((FINAL_RADIUS/r0) - 1)
mad r1, r1, c1, r2     ; r1 = 1+t*((FINAL_RADIUS/r0) - 1)
 
mul r2.xyz, v0.xyz, r1.x
 
m4x4 r1, r2, c27        ; position
m4x4 r3, r1, c31        ; projection to plane


rcp r7, r3
mul r3, r3, r7.w
;color
; calculating direction vector
add r11, c17, -r3       ; r11 = position(point) - position(vertex) = d
dp3 r4, r11, r11        ; r4 = d^2
rsq r7, r4              ; r7 = 1/d
; calculating attenuation
dst r2, r4, r7          ; r2 = (1, d, d^2, 1/d)
dp3 r0, r2, c35         ; r0 = (a + b*d + c*d^2)
mul r2, r11, r2
rcp r0, r0              ; r0 = attenuation!
mov r10, c100           ; r10 = black
mul r10.a, c111.a, r0.a ; r10.alpha = 1 * attenuation
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
m4x4 oPos, r3, c2
mov oD0, r10
