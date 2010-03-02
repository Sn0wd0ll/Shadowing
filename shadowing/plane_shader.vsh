vs_1_1
dcl_position v0
dcl_color v1
dcl_normal v3
 
;;;;;;;;;;;;;;CONSTANTS;;;;;;;;;;;;;
; c0 - c3 is view matrix           
; c14 is diffuse coefficient       
; c15 is ambient light color       
; c16 is point light color         
; c17 is point light position      
; c18 are attenuation constants    
; c19 is specular coefficient      
; c20 is specular constant 'f'     
; c21 is eye position              
; c27 - c30 is position matrix    
 
def c100, 0.0, 0.0, 0.0, 0.0
def c111, 1.0, 1.0, 1.0, 1.0
 
;position
m4x4 r1, v0, c27                  
;normals
m4x4 r10, v3, c27                 

;;;;;;;;;;;;;;;;;;;; Lighting ;;;;;;;;;;;;;;;;;;;;;;;;;;;; 
; calculating normalized v
add r9, c21, -r1       ; r9 = position(eye) - position(vertex)
dp3 r0, r9, r9         ; r0 = d^2
rsq r7, r0             ; r7 = 1/d
mul r9, r9, r7.x       ; normalize r9
 
;;;;;;;;;;;;;;;;;;;;;;;; Point ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; calculating normalized direction vector
add r11, c17, -r1       ; r11 = position(point) - position(vertex)
dp3 r2, r11, r11        ; r2 = d^2
rsq r7, r2              ; r7 = 1/d
mul r11, r11, r7.x      ; normalize r11
; calculating cos(theta)
dp3 r5, r11, r10        ; r5 = cos(theta)
; calculating attenuation
dst r2, r2, r7          ; r2 = (1, d, d^2, 1/d)
dp3 r0, r2, c18         ; r0 = (a + b*d + c*d^2)
rcp r0, r0              ; r0 = attenuation coef
;diffuse
mul r4, c16, r5.x       ; r4 = I(point)*cos(theta)
mul r4, r4, c14.x        ; r4 *= coef(diffuse)
mul r4, r4, r0.x        ; r4 *= attenuation
 
max r6, r4, c100        ; checking negative color
 
;;;;;;;;;;;;;;;;;;;;;;; Ambient ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
add r6, r6, c15         ; r6 += I(ambient)
 
;;;;;;;;;;;;;;;;;;;;;;;; Results ;;;;;;;;;;;;;;;;;;;;;;;;;;;;
m4x4 oPos, r1, c0
mul oD0, v1, r6