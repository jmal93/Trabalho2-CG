# Trabalho 1 - Computação Gráfica

### Como rodar
Primeiramente crie um diretório `build/` dentro do projeto:
```
mkdir build
```
Depois, execute os seguintes comandos:
```
cmake -B build -S .
make -C build/
```
Para executar, execute esse comando:
```
./build/main_2d
```
Toda vez que fizer alguma alteração no código, deve executar `make -C build/` para compilar o projeto novamente. 
> [!WARNING]  
> Certifique que esteja fazendo isso dentro do diretório raiz do projeto, se fizer dentro de outro diretório faz compilar com erro pelo fato do executável não achar diretórios como shader e images
