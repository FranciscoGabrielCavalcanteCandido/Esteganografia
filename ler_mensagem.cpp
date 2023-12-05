#include <iostream>
#include <cstring>
#define STB_IMAGE_IMPLEMENTATION
#include "C:\src\c++\esteganografia_BMP\stb-master\stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "C:\src\c++\esteganografia_BMP\stb-master\stb_image_write.h"

using namespace std;

template <typename T>
inline T extract_bits(const T v, const uint32_t bstart, const uint32_t blength)
{
    const T mask = (1 << blength) - 1;
    return (v >> bstart) & mask;
}

template <typename T>
inline T set_bits(const T v, const uint32_t bstart, const uint32_t blength, const T bits)
{
    const T mask = ((1 << blength) - 1) << bstart;

    return (v & ~mask) | (bits << bstart);
}

bool carregarImagem(const char *arquivo_imagem, int &largura, int &altura, int &canais, unsigned char *&dados)
{
    dados = stbi_load(arquivo_imagem, &largura, &altura, &canais, 0);
    if (!dados)
    {
        std::cerr << "Erro ao carregar a imagem" << std::endl;
        return false;
    }
    else
    {
        std::cerr << "Imagem carregada" << std::endl;
        return true;
    }
}

void extrairMensagemDaImagem(unsigned char *dados, int largura, int altura, int canais)
{
    char mensagem_recuperada[256];
    int indice_mensagem = 0;
    int tamanho_mensagem = dados[0];

     std::cout << "O tamanho da mensagem escondida e: " << tamanho_mensagem << std::endl;
    for (int i = canais; i < largura * altura * canais; i += canais)
    {
        if (indice_mensagem < tamanho_mensagem)
        {
            unsigned char caractere;

            unsigned char vermelho = dados[i];
            unsigned char verde = dados[i + 1];
            unsigned char azul = dados[i + 2];

            unsigned char parte_mensagem_1 = extract_bits(vermelho, 0, 2);
            unsigned char parte_mensagem_2 = extract_bits(verde, 0, 3);
            unsigned char parte_mensagem_3 = extract_bits(azul, 0, 3);

            caractere = set_bits(caractere, 0, 2, parte_mensagem_1);
            caractere = set_bits(caractere, 2, 3, parte_mensagem_2);
            caractere = set_bits(caractere, 5, 3, parte_mensagem_3);

            mensagem_recuperada[indice_mensagem] = caractere;

            indice_mensagem++;
        }
        else
        {
            break;
        }
    }
    mensagem_recuperada[indice_mensagem] = 0;

    cout << "Mensagem recuperada: " << mensagem_recuperada << endl;
}

int main()
{
    int largura, altura, canais;
    const char *arquivo_imagem = "C:/src/c++/esteganografia_BMP/imagens/imagem_modificada.bmp";
    unsigned char *dados;

    if (!carregarImagem(arquivo_imagem, largura, altura, canais, dados))
    {
        return 1;
    }

    extrairMensagemDaImagem(dados, largura, altura, canais);

    stbi_image_free(dados);

    return 0;
}
