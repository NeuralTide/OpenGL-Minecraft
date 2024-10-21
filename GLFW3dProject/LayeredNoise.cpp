
#include "LayeredNoise.h"
#include "SimplexNoise.h"

float LayeredNoise::getCaveNoise(int x, int y, int z) {
    float n1 = SimplexNoise::noise(x * .008, z * .008);
    float fact = 0;

    if (n1 > -.3) {
        fact = 10;
    }

    return SimplexNoise::noise(x * .01, y * .01, z * .01) + SimplexNoise::noise(x * .01, y * .04, z * .01); +SimplexNoise::noise(x * .04, y * .01, z * .04) + fact;

}

float LayeredNoise::getCaveNoise_2(int x, int y, int z) {

    return SimplexNoise::noise(x * .06, y * .01, z * .06);
}

int LayeredNoise::getHeightNoise(int x, int z) {
    int yLevel = 0;


    int n1 = (int)((SimplexNoise::noise(x / 20, z / 20) + 1));
    int n2 = (int)((SimplexNoise::noise((x + 8) / 20, (z + 8) / 20) + 1));
    int n3 = (int)((SimplexNoise::noise((x + 16) / 20, (z + 16) / 20) + 1));

    int n4 = (int)((SimplexNoise::noise((x + 32) / 20, (z + 32) / 20) + 1));
    int n5 = (int)((SimplexNoise::noise((x + 48) / 20, (z + 48) / 20) + 1));
    int n6 = (int)((SimplexNoise::noise((x + 64) / 20, (z + 64) / 20) + 1));


    int n7 = (int)((SimplexNoise::noise(x / 10, z / 10) + 1));
    int n8 = (int)((SimplexNoise::noise((x + 8) / 10, (z + 8) / 10) + 1));
    int n9 = (int)((SimplexNoise::noise((x + 16) / 10, (z + 16) / 10) + 1));

    int n10 = (int)((SimplexNoise::noise((x + 32) / 10, (z + 32) / 10) + 1));
    int n11 = (int)((SimplexNoise::noise((x + 48) / 10, (z + 48) / 10) + 1));
    int n12 = (int)((SimplexNoise::noise((x + 64) / 10, (z + 64) / 10) + 1));

    float mod = (SimplexNoise::noise(x * .006, z * .006));


    int noise = (int)(((((n1 + n2 + n3 + n4 + n5 + n5 + n6 + n7)) + ((n8 + n10 + n11 + n12)) + 2)));
    if (mod > 0) {
        mod = mod * 6;
        noise = noise + (int)(((((n1 + n2 + n3 + n4 + n5 + n5 + n6 + n7)) + ((n8 + n10 + n11 + n12)) + 2)) * mod);
    }







    return noise;
}
