#include <iostream>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include "C:\src\c++\esteganografia_BMP\stb-master\stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "C:\src\c++\esteganografia_BMP\stb-master\stb_image_write.h"

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

bool ocultarMensagemNaImagem(unsigned char *dados, int largura, int altura, int canais, const std::string &mensagem)
{
    int indice_mensagem = 0;
    for (int i = 0; i < largura * altura * canais; ++i)
    {
        if (indice_mensagem < mensagem.length())
        {
            unsigned char bit_mensagem = (mensagem[indice_mensagem] >> (7 - (i % 8))) & 1;
            dados[i] = (dados[i] & 0xFE) | bit_mensagem;
            ++indice_mensagem;
        }
        else
        {
            break;
        }
    }
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

std::string recuperar_mensagem_da_imagem(const unsigned char *dados_imagem, int largura, int altura, int canais)
{
    std::string mensagem;
    char caractere = 0;
    int bit_atual = 0;

    for (int i = 0; i < largura * altura * canais; ++i)
    {
        // Extrai o bit menos significativo do canal de cor
        unsigned char bit_mensagem = dados_imagem[i] & 1;

        // Adiciona o bit ao caractere (7 bits formam um caractere, o 8º bit é sempre 0)
        caractere = (caractere << 1) | bit_mensagem;

        // Incrementa o índice do bit atual
        ++bit_atual;

        // Se formou um caractere completo, adiciona à mensagem
        if (bit_atual == 7)
        {
            mensagem.push_back(caractere);
            caractere = 0;
            bit_atual = 0;

            // Se o último caractere for '\0', termina a leitura da mensagem
            if (mensagem.back() == '\0')
                break;
        }
    }

    return mensagem;
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

    std::string mensagem_ocultar = "ola";
    mensagem_ocultar.push_back('\0');

    if (ocultarMensagemNaImagem(dados, largura, altura, canais, mensagem_ocultar))
    {
        const char *nova_imagem = "C:/src/c++/esteganografia_BMP/imagens/imagem_modificada.bmp";
        salvarImagemComMensagem(nova_imagem, largura, altura, canais, dados);
    }

    std::string mensagem_escondida = recuperar_mensagem_da_imagem(dados, largura, altura, canais);

    // Imprimir a mensagem
    if (mensagem_escondida.empty())
    {
        std::cout << "Nenhuma mensagem encontrada na imagem." << std::endl;
    }
    else
    {
        std::cout << "Mensagem: " << mensagem_escondida << std::endl;
    }

    stbi_image_free(dados);
   
}