//Função sombreamento (Phong Shading)
vec3 sombreamento(void);

//variaveis do tipo uniform são valores constantes para todos os fragmentos ou vértices do shader
uniform vec3 luzPos;
uniform vec3 luzColor;
uniform vec3 eyePos;

//valores recebidos do vertex shader através de interpolação linear (devem ter o mesmo nome)
varying vec3 fragPos;
varying vec3 Normal;
varying vec3 color;

void main(void) {
    vec3 corFinal = sombreamento();
    //a variável interna gl_FragColor deve receber a cor que o fragmento irá aparecer na tela
    //todo fragment shader deve atribuir uma cor a essa variavel no final. Neste caso, a cor calculada pela função de sombreamento
    gl_FragColor = vec4(corFinal, 1.0);
}

vec3 sombreamento() {
    float intensidadeAmb = 0.05;
    vec3 ambiente  = intensidadeAmb * luzColor;

    vec3 norm = normalize(Normal);
    vec3 luzDir = normalize(luzPos - fragPos);
    float dif = max(dot(norm, luzDir), 0.0);
    vec3 difusa = dif * luzColor;

    vec3 especular;
    if(dot(norm, luzDir) < 0.0) {
	    especular = vec3(0.0, 0.0, 0.0);
    }
    else {
    	float intensidadeEspec = 0.25;
    	vec3 eyeDir = normalize(eyePos - fragPos); //o ponto está em coordenada de câmera, entao o vetor eyeDir é calculado assim
    	// vec3 reflexDir = reflect(-luzDir, Normal);
        vec3 halfwayDir = normalize(luzDir + eyeDir); 
    	float espec = pow(max(dot(norm, halfwayDir), 0.0), 32.0);
    	especular = espec * intensidadeEspec * luzColor;
    }
    
    vec3 corFinal = (ambiente + difusa) * color + especular; //somando todas as componentes

    return corFinal;
}
