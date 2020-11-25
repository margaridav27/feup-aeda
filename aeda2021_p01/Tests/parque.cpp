#include "parque.h"
#include <vector>

using namespace std;

ParqueEstacionamento::ParqueEstacionamento(unsigned int lot, unsigned int nMaxCli): lotacao (lot), numMaximoClientes(nMaxCli) {
    clientes.clear(); // o parque encontra-se inicialmente vazio
    vagas = lot;
}
bool ParqueEstacionamento::adicionaCliente(const string & nome) {
    if (clientes.size() == numMaximoClientes) return 0; // foi atingido o número máximo de clientes
    else{
        InfoCartao novoCliente;
        novoCliente.nome = nome;
        novoCliente.presente = false; // o cliente está inicialmente fora do parque
        clientes.push_back(novoCliente);
        return 1;
    }
}
bool ParqueEstacionamento::retiraCliente(const string & nome) {
    for (size_t i = 0; i < clientes.size(); i++){
        if (clientes[i].nome == nome){ // o cliente existe
            if (clientes[i].presente == false){ // e está fora do parque
                clientes.erase(clientes.begin() + i);
                return 1;
            }
        }
    }
    return 0;
}
bool ParqueEstacionamento::entrar(const string & nome) {
    if (vagas == 0) return 0; // não há lugares disponíveis no estacionamento
    int pos = posicaoCliente(nome);
    if (pos != -1) { // o cliente existe
        if (clientes[pos].presente == false) { // e está fora do parque
            clientes[pos].presente = true;
            vagas--;
            return 1;
        }
    }
    return 0;
}
bool ParqueEstacionamento::sair(const string & nome) {
    int pos = posicaoCliente(nome);
    if (pos != -1) { // o cliente existe
        if (clientes[pos].presente == true) { // e está dentro do parque
            clientes[pos].presente = false;
            vagas++;
            return 1;
        }
    }
    return 0;
}
int ParqueEstacionamento::posicaoCliente(const string & nome) const {
    for (size_t i = 0; i < clientes.size(); i++){
        if(clientes[i].nome == nome) return i;
    }
    return -1;
}
unsigned ParqueEstacionamento::getNumLugares() const { return lotacao; }
unsigned ParqueEstacionamento::getNumMaximoClientes() const { return numMaximoClientes; }
unsigned ParqueEstacionamento::getNumLugaresOcupados() const { return (lotacao - vagas); }
unsigned ParqueEstacionamento::getNumClientesAtuais() const { return clientes.size(); }




