


This project was developed for Visualização e Iluminação (Visualization and Lighting) class at Universidade do Minho. The main goal of this project is to implement a modular rendering engine in C++ following the teachings of the Physically Based Rendering: From theory to implementation book by Matt Pharr, Wenzel Jakob and Greg Humphreys. This has been my most insightful project into the rendering world, where I've learned about the theory behind the rendering process of which I was already familiarized with most concepts due to previous experience in Blender.








tentei implementar um algoritmo que pre calculava o peso de cada luz para cada objeto com base na distancia entre a luz e o ponto medio da boundign box e depois usar isso para escolher com maior probabilidade as luzes mais proximas mas isto nao deu certo a nivel de eficiencia entao deixei de parte.




A minha ideia atual:
- a funcao de dar load a cena faz de tal modo que cada faceID seja unico e em sequencia
- Posso deste modo guardar de alguma maneira num mapa de idx da mesh como chave e como valor o par de id de começo e o id de fim das faces que pertencem a essa mesh
- vou ter q