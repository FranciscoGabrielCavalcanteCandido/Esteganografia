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

    for (int i = 0; i < largura * altura * canais; i += 3)
    {
        if (indice_mensagem < 256)
        {
            unsigned char verde = dados[i + 1];
            char parte_mensagem = static_cast<char>(extract_bits(verde, 0, 2) + '0'); // Converte para caractere ASCII

            mensagem_recuperada[indice_mensagem] = parte_mensagem;

            if (parte_mensagem == '\0') // Verifica se encontrou o caractere nulo, indicando o final da mensagem
                break;

            indice_mensagem++;
        }
        else
        {
            break;
        }
    }

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
