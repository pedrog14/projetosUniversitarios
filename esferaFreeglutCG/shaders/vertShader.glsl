//varying é a instrução indicando que essas variáveis serão enviadas ao fragment shader
//seus conteúdos são por vértice e serão interpoladas linearmente para cada fragmento
//por este motivo, seus valores não serão exatamente iguais no fragment shader
varying vec3 fragPos;
varying vec3 Normal;
varying vec3 color;

//todo shader precisa de ao menos uma função main()
void main(void) {
    fragPos = vec3(gl_Vertex); //pegando o valor enviado através do comando glVertex3f()
    Normal  = gl_Normal;       //pegando o valor enviado através do comando glNormal3f()
    color   = vec3(gl_Color);        //pegando o valor enviado através do comando glColor3f()

    //gl_Position é a variavel interna do OpenGL que deve receber a posição do vértice após todas as transformações geométricas (locais, câmera e projeção)
    //a variavel gl_ModelViewProjectionMatrix armazena a combinação de todas as matrizes definidas no seu código
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
