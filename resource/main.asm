FIRST LINE
_declspec(align(16)) float v_pfArrayA[] = {100.0, 12.3, 524.5, 102.9}
movaps xmm0, v_pfArrayA
movss xmm1, xmm0
divps xmm0, v_pfArrayA
addss xmm1, xmm0
cmpeqps xmm6, xmm7