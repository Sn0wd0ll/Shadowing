vs_1_1
dcl_position v0
dcl_color v1
dcl_texcoord v2
dcl_normal v3

def c100, 0.0, 0.0, 0.0, 0.0
def c111, 1.0, 1.0, 1.0, 1.0
;;;;;;;;;;Constants;;;;;;;;;;;;;;;;;;;
; c0 - view matrix           
; c4 - 1 bone matrix       
; c8 - 2 bone matrix      
; c17- point light position      
; c27- position matrix    
; c31- shadow projective matrix   
; c35- shadow attenuation          
 
;;;;;;;;;;;;;;;;;;;;;; Skinning ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;position 
m4x4 r1, v0, c4
mul r1.xyz, r1.xyz, v2.x            ; first bone
m4x4 r0, v0, c8
mad r0.xyz, r0.xyz, v2.y, r1.xyz    ; second bone
m4x4 r1, r0, c27                    ; position
m4x4 r3, r1, c31					; projection to plane

rcp r7, r3
mul r3, r3, r7.w
 
m4x4 oPos, r3, c0
;color
;direction vector
add r11, c17, -r3       ; r11 = position(point) - position(vertex) = d
dp3 r4, r11, r11        ; r4 = d^2
rsq r7, r4              ; r7 = 1/d
;attenuation
dst r2, r4, r7          ; r2 = (1, d, d^2, 1/d)
mul r2, r11, r2
dp3 r0, r2, c35         ; r0 = (a + b*d + c*d**2)
rcp r0, r0              ; r0 = attenuation!
mov r10, c100           ; r10 = black
mul r10.a, c111.a, r0.x ; r10.alpha = 1 * attenuation

;-------------------------------------------------------------;
mov oD0, r10
