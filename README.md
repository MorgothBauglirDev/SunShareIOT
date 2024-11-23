# SunShareIOT
## Solução:

SunShare IoT - Simulador de Estimativa de Geração Solar
Bem-vindo ao SunShare IoT, uma solução de IoT para monitoramento climático e estimativa de produção energética de placas solares. O projeto utiliza sensores conectados a uma placa controladora ESP32 para capturar dados do clima e publica essas informações via protocolo MQTT. Com esses dados, podemos prever a geração de energia solar em kWh, levando em consideração fatores como luminosidade, temperatura e umidade.

Funcionalidades
Coleta Climática:
LDR (Sensor de Luminosidade): Monitora a intensidade luminosa.
DHT22 (Sensor de Temperatura e Umidade): Captura temperatura e umidade relativa do ar.
Estimativa de Produção Solar:
Com base nos dados coletados, calcula uma previsão da energia gerada em kWh.
Transmissão MQTT:
Publica os dados para um broker MQTT, permitindo integração com outras plataformas.
Visualização em Dashboard:
Utiliza o Node-RED para criar gráficos em tempo real, exibir dados numéricos e monitorar tendências.
Pré-requisitos
Antes de iniciar, você precisará de:

Plataforma de Simulação:
Wokwi Simulator para emular o funcionamento do ESP32 e dos sensores.
Node-RED
Broker MQTT:
HiveMQ
Instruções para Replicar o Projeto
1. Simulação no Wokwi
Arquivo do Projeto Wokwi
Acesse o Wokwi Simulator.
Substitua o conteudo de sketch.ino e diagram.json

Instale as bibliotecas que estão sendo importadas pelo Library Manager:
Arduino.h
WiFi.h
ArduinoJson.h
DHTesp.h
PubSubClient.h

Leitura dos sensores DHT22 e LDR.
Publicação dos dados no broker MQTT usando a biblioteca PubSubClient.
2. Configuração do Fluxo no Node-RED
Fluxo do Node-RED
Importe o arquivo: fluxo.json na interface do NODE-RED

Certifique-se de que:
O nó MQTT está configurado para o mesmo broker usado no ESP32.
O dashboard está ativo e configurado para exibir gráficos e informações numéricas.
Visualização no Dashboard
Gráfico de Geração x Consumo: Mostra a previsão de geração solar e consumo.
Créditos de Energia: Exibe a diferença entre geração e consumo.
3. Execução
Deploy o fluxo no Node-RED e acesse http://127.0.0.1:1880/ui para visualizar o Dashboard.
No Wokwi, inicie a simulação do ESP32. Os dados começarão a ser publicados no broker MQTT.
Verifique o dashboard do Node-RED para acompanhar os dados em tempo real.
