#define STB_IMAGE_IMPLEMENTATION
#include "C:\src\c++\stb-master\stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "C:\src\c++\stb-master\stb_image_write.h"
#include <iostream>

template <typename T>
inline T extract_bits(const T v, const unsigned bstart, const unsigned blength)
{
    const T mask = (1 << blength) - 1;

    return ((v >> bstart) & mask);
}

template <typename T>
inline T set_bits(const T v, const unsigned bstart, const unsigned blength, const T bits)
{
    const T mask = ((1 << blength) - 1) << bstart;

    return (v & mask) | (bits << bstart);
}

int main()
{

    int largura, altura, canais;
    const char* arquivo_imagem = "C:/src/c++/esteganografia_BMP/download.bmp";
    unsigned char *dados_imagem = stbi_load(arquivo_imagem, &largura, &altura, &canais, 0);

    if (!dados_imagem)
    {
        std::cerr << "Erro ao carregar a imagem" << std::endl;
        return 1;
    }
    else
    {
        std::cerr << "Imagem carregada" << std::endl;
    }

    // int posicao_x =10;
    // int posicao_y = 20;

    // int indice_pixel = (posicao_x * largura + posicao_y)*canais;
    // std::cerr << indice_pixel << std::endl;

    // unsigned char valor_canal_vermelho = dados_imagem[indice_pixel];

    // std::cout << "Valor do canal vermelho (R) do pixel na posição (" << posicao_x << ", " << posicao_y << "): " << static_cast<int>(valor_canal_vermelho) << std::endl;


    // const char *nova_imagem_bmp = "C:/src/c++/esteganografia_BMP/nova_imagem.bmp"; // Substitua pelo caminho e nome desejado do novo arquivo BMP.

    // if (!stbi_write_bmp(nova_imagem_bmp, largura, altura, canais, dados_imagem))
    // {
    //     std::cerr << "Erro ao salvar a imagem em um novo arquivo BMP" << std::endl;
    // }
    // else
    // {
    //     std::cout << "Imagem salva com sucesso em " << nova_imagem_bmp << std::endl;
    //     std::cout << "largura " << largura << std::endl;
    //     std::cout << "altura " << altura << std::endl;
    //     std::cout << "canais " << canais << std::endl;
    // }

    stbi_image_free(dados_imagem);
    return 0;
}