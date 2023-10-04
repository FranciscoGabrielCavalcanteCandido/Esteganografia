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

    std::string mensagem;
    std::cout << "Digite sua mensagem: ";
    std::cin.ignore();
    std::getline(std::cin, mensagem);
    std::cout << mensagem<< std::endl;

    int indice_mensagem = 0;
    for (int i = 0; i < largura * altura * canais; ++i)
    {   
        std::cout << mensagem[indice_mensagem] << std::endl;
        if (indice_mensagem <= mensagem.length())
        {
            unsigned char bit_mensagem = (mensagem[indice_mensagem] >> (7 - (i % 8))) & 1;
            dados_imagem[i] = (dados_imagem[i] & 0xFE) | bit_mensagem;
            ++indice_mensagem;
        }
        else
        {
            break;
        }
         
    }
    const char* nova_imagem = "C:/src/c++/esteganografia_BMP/imagem_modificada.bmp";
     int resultado = stbi_write_bmp(nova_imagem, largura, altura, canais, dados_imagem);

    if (resultado != 0)
    {
        std::cout << "Mensagem escondida na imagem com sucesso." << std::endl;
    }
    else
    {
        std::cout << "Erro ao salvar a imagem com a mensagem." << std::endl;
    }

    stbi_image_free(dados_imagem);
    return 0;
}