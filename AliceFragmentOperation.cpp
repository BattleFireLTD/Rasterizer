#include "AliceFragmentOperation.h"
static Auint SRC_ALPHA_ONE_MINUS_SRC_ALPHA = A_SRC_ALPHA | A_ONE_MINUS_SRC_ALPHA;
static Auint BlendColorSRC_ALPHA_ONE_MINUS_SRC_ALPHA(Abyte src_r, Abyte src_g, Abyte src_b, Abyte src_a,
			Abyte dst_r, Abyte dst_g, Abyte dst_b, Abyte dst_a) {
	float alpha = float(src_a) / 255.0f;
	Abyte nr = Abyte(float(src_r)*alpha + (1.0f - alpha)*float(dst_r));
	Abyte ng = Abyte(float(src_g)*alpha + (1.0f - alpha)*float(dst_g));
	Abyte nb = Abyte(float(src_b)*alpha + (1.0f - alpha)*float(dst_b));
	nr = nr > 255 ? 255 : nr;
	ng = ng > 255 ? 255 : ng;
	nb = nb > 255 ? 255 : nb;
	return MakeColor(nr,ng,nb,255);
}
Auint BlendColor(Abyte src_r, Abyte src_g, Abyte src_b, Abyte src_a, Auint src_option,
	Abyte dst_r, Abyte dst_g, Abyte dst_b, Abyte dst_a, Auint dst_option) {
	Auint option = src_option | dst_option;
	if (option== SRC_ALPHA_ONE_MINUS_SRC_ALPHA){
		return BlendColorSRC_ALPHA_ONE_MINUS_SRC_ALPHA(src_r, src_g, src_b, src_a, dst_r, dst_g, dst_b, dst_a);
	}
	return 0;
}