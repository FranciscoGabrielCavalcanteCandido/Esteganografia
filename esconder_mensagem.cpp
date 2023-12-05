#include <iostream>
#include <string>
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

bool ocultarMensagemNaImagem(unsigned char *dados, int largura, int altura, int canais, char *mensagem)
{
    int indice_mensagem = 0;
    int tamanho_mensagem = strlen(mensagem);
    dados[0] = tamanho_mensagem;

    for (int i = canais; i < largura * altura * canais; i += canais)
    {
        unsigned char vermelho = dados[i];
        unsigned char verde = dados[i + 1];
        unsigned char azul = dados[i + 2];
        if (indice_mensagem <= tamanho_mensagem)
        {
            unsigned char caractere;
            caractere = mensagem[indice_mensagem];

            unsigned char parte_mensagem_1 = extract_bits(caractere, 0, 2);
            unsigned char parte_mensagem_2 = extract_bits(caractere, 2, 3);
            unsigned char parte_mensagem_3 = extract_bits(caractere, 5, 3);

            vermelho = set_bits(vermelho, 0, 2, parte_mensagem_1);
            verde = set_bits(verde, 0, 3, parte_mensagem_2);
            azul = set_bits(azul, 0, 3, parte_mensagem_3);

            dados[i] = vermelho;
            dados[i + 1] = verde;
            dados[i + 2] = azul;

            if (indice_mensagem == tamanho_mensagem)
            {

                break;
            }
        }
        else
        {
            break;
        }

        indice_mensagem++;
    }

    std::cout << "O tamanho da mensagem é: " << tamanho_mensagem << std::endl;
    return true;
}

bool salvarImagemComMensagem(const char *nova_imagem, int largura, int altura, int canais, unsigned char *dados)
{
    int resultado = stbi_write_bmp(nova_imagem, largura, altura, canais, dados);
    if (resultado != 0)
    {
        std::cout << "Mensagem escondida na imagem com sucesso." << std::endl;
        return true;
    }
    else
    {
        std::cout << "Erro ao salvar a imagem com a mensagem." << std::endl;
        return false;
    }
}

int main()
{
    int largura, altura, canais;
    const char *arquivo_imagem = "C:/src/c++/esteganografia_BMP/imagens/download.bmp";
    unsigned char *dados;

    if (!carregarImagem(arquivo_imagem, largura, altura, canais, dados))
    {
        return 1;
    }

    char *mensagem_ocultar = new char[256];
    cout << "Informe a mensagem que deseja esconder: ";
    cin.getline(mensagem_ocultar, 256);

    if (ocultarMensagemNaImagem(dados, largura, altura, canais, mensagem_ocultar))
    {
        const char *nova_imagem = "C:/src/c++/esteganografia_BMP/imagens/imagem_modificada.bmp";
        salvarImagemComMensagem(nova_imagem, largura, altura, canais, dados);
    }

    stbi_image_free(dados);
}