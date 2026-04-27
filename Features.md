# SimpleOS x64 Ultimate - Feature Roadmap

Este arquivo documenta todas as funcionalidades implementadas no SimpleOS x64, servindo como referência técnica para o estado atual do projeto.

## 🏗️ Arquitetura de Núcleo (Kernel Core)
- [x] **x86_64 Long Mode Nativo:** Operação completa em 64 bits.
- [x] **Paging de 4 Níveis (PML4):** Gerenciamento de memória virtual moderno com suporte a endereços de 64 bits.
- [x] **TSS (Task State Segment):** Infraestrutura pronta para troca de contexto e isolamento Ring 0/Ring 3.
- [x] **GDT & IDT x64:** Tabelas de descritores e interrupções configuradas para o padrão de 64 bits.
- [x] **Hybrid Executive Dispatcher:** Suporte inicial para syscalls estilo Linux (`0x80`) e Windows NT (`0x2E`).

## 💾 Gerenciamento de Sistema (Executive Managers)
- [x] **NT Object Manager:** Gerenciamento de recursos via Handles (Arquivos, Processos, Eventos).
- [x] **Executive Pool (kmalloc):** Alocação dinâmica de memória com suporte a Tags (`ExAllocatePoolWithTag`).
- [x] **I/O Manager (WDM Style):** Arquitetura baseada em `DRIVER_OBJECT` e `DEVICE_OBJECT`.
- [x] **Sistema de IRP (I/O Request Packets):** Comunicação entre subsistemas via pacotes de requisição oficiais do Windows.
- [x] **Virtual File System (VFS):** Camada de abstração que permite montar diferentes sistemas de arquivos.

## 🔌 Drivers e Hardware
- [x] **PCI Bus Manager:** Scan automático do barramento de hardware.
- [x] **ATA PIO Driver:** Suporte a leitura e escrita em discos IDE/SATA (Modo Legado).
- [x] **VGA Text Mode Advanced:** Suporte a hardware cursor, cores profissionais e Scrolling nativo.
- [x] **PS/2 Keyboard Driver:** Captura de entrada via polling (pronto para migração para interrupções).
- [x] **Serial Logging (COM1):** Logs em tempo real enviados para o host (QEMU).
- [x] **Wifi/Ethernet Stubs:** Estrutura preparada para drivers de rede NDIS.

## 🖥️ Interface e Experiência do Usuário
- [x] **Professional Desktop Environment:** Interface clássica "White on Blue" com barra de tarefas simulada.
- [x] **Interactive Shell:** Interpretador de comandos com suporte a PATH.
- [x] **SimpleOS Editor (Nano-lite):** Editor de texto integrado com buffer de múltiplas linhas.
- [x] **Package Manager Simulator (Apt-style):** Sistema de instalação de software via comandos.
- [x] **KSOD (Kernel Screen of Death):** Tela de erro crítica com snapshot completo de registros x64.

## 📦 Formatos de Executáveis
- [x] **PE Loader (Windows .exe):** Parsing real de cabeçalhos PE32+ e identificação de seções.
- [x] **DLL Dependency Checker:** Identificação de bibliotecas dinâmicas necessárias por um binário.
- [x] **GPT Support:** Preparado para ler tabelas de partição GUID.

## 🛡️ Segurança e Estabilidade
- [x] **IRQL Management:** Sistema de níveis de prioridade de interrupção (PASSIVE até DIRQL).
- [x] **Validation Pipeline:** GitHub Actions com auditoria de segurança e linting automático.
- [x] **Hybrid ISO:** Imagem bootável em BIOS, UEFI e Pendrives (compatível com Rufus).

---
*Última atualização: 27 de Abril de 2026*
