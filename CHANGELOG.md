# Changelog

Todas as alteracoes notaveis neste projeto serao documentadas neste arquivo.

O formato e baseado em [Keep a Changelog](https://keepachangelog.com/pt-BR/1.1.0/),
e este projeto adere ao [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Adicionado

- **Camera:** sistema de zoom com Ctrl+Scroll do mouse (niveis -4 a +4,
  distancia 600 a 1400) e reset com Ctrl+clique da roda
- **Camera:** centralizacao automatica no torso do personagem
  (CAMERA_BODY_OFFSET = 80.f) ao inves dos pes
- **Buff visual:** efeito de aura de ataque (luz vermelha + flare +
  particulas) para summons, incluindo `eBuff_Attack`, `eBuff_HelpNpc`,
  `eBuff_Att_up_Ourforces`
- **Buff visual:** efeito de aura de defesa (`eBuff_Defense`) para summons
- **Buff visual (`eBuff_Attack`):** efeito de luz + flare no
  `InsertBuffPhysicalEffect` para objetos que nao sao `MODEL_PLAYER`
- **Notificacoes:** mensagens do sistema (`ReceiveNotice`) agora
  aparecem tambem no chat (`g_pChatListBox`)
- **NPC Dialogue:** suporte a NPC de buff (index 257) com opcao de
  buff direto no dialogo
- **Infra:** `docker-compose.build.yml` e `docker/mingw-builder.Dockerfile`
  para build cross-compilado via Docker
- **CI/CD:** workflow de build manual do MuMain (`.github/workflows/build.yml`)
- **CI/CD:** workflow de build Windows com artefato (`ci-build.yml`)
- **CI/CD:** workflow de release automatica ao criar tag (`release.yml`)
- **CI/CD:** workflow de build MinGW para desenvolvimento (`mingw-build.yml`,
  `mingw-build-dev.yml`)
- **CI/CD:** artefato leve de patch para atualizacoes mais rapidas do cliente
- **Docs:** `README.md` com secoes de CI/CD e instrucoes de build
- **Docs:** este arquivo `CHANGELOG.md`

### Corrigido

- **Camera:** zoom nao funcionava porque `CalculateCameraPosition()` sempre
  resetava `g_shCameraLevel` para 0
- **Camera:** ao dar zoom in, o chao desaparecia porque `CreateFrustrum2D()`
  nao aceitava niveis negativos de camera
- **Camera:** personagem se deslocava na tela ao zoomar porque o Z da camera
  era fixo (C - 150); agora escala proporcionalmente (C * 0.85)
- **Quest (CSQuest):** `m_byCurrQuestIndex` (`uint8_t`) ao decrementar em 0
  wrappava para 255, causando crash no acesso ao array de quests
- **Quest (UIControls/NewUI*):** `questNumber` e `questGroup` extraidos com
  mascaras de bit trocadas (high byte vs low byte) em 5 locais diferentes
- **Quest (QuestMng):** `SetQuestRequestReward()` usava struct-casting direto
  no buffer do pacote, sem validacao de tamanho; refatorado para leitura
  byte-a-byte com `read_u16`/`read_u32` e bounds checking
- **Quest (WSclient):** `ReceiveQuestState`/`ReceiveQuestResult` agora usam
  `ApplyLegacyQuestStateBlock()` que decodifica corretamente estados
  compactados sem travar a janela de quest no indice 3
- **Quest (WSclient):** `ReceiveQuestByNPCEPList` com validacao de
  `m_wQuestCount` contra tamanho real do pacote
- **Quest (WSclient):** `ReceiveQuestQSRequestReward`/`ReceiveProgressQuestRequestReward`
  com validacao de tamanho de pacote antes de processar
- **Quest:** `SendQuestSelectRequest()` e `SendQuestIndexByEtcSelection()`
  enviavam questNumber/questGroup com os nibbles trocados (LOWORD vs HIWORD)
- **Skill (ReceiveMagicList):** faltava `Offset += 7;` no branch `ListType 0x02`,
  causando leitura incorreta de skills subsequentes
- **Skill (WSclient):** `ReceiveMagicList()` nao reconstruia a lista de skills
  ao receber atualizacao completa (`ListType == 0`), deixando skills obsoletas
  ativas
- **Skill (CSItemOption):** `IsNonWeaponSkillOrIsSkillEquipped()` nao verificava
  se a skill ja estava na lista de skills aprendidas, impedindo uso de skills
  de arma que o cliente nao associava ao item equipado
- **Skill (SkillManager):** verificacao de requisitos de skill usava as
  estruturas de classe diretamente, sem normalizar via `CharacterAttribute`,
  causando falso-negativo em algumas condicoes
- **NPC Dialogue:** `SetCurNPCWords()` com protecao contra `pszSrc == NULL`
- **NPC Dialogue:** `SetQuestListText()` com clamping de `nIndexCount` e
  protecao contra `adwSrcQuestIndex == nullptr`

### Alterado

- **Nomes de personagem:** permitido uso de underscore (`_`) e tamanho minimo
  reduzido para 3 caracteres (`CharMakeWin.cpp`, `Local.cpp`)
- **CI/CD:** workflows migrados de MSBuild para MinGW (cross-compile)
- **CI/CD:** builds Windows alterados de x64 para x86 (compatibilidade com
  cliente original)
- **Quest:** bitfields de `questNumber`/`questGroup` normalizados em todo o
  codigo: `questNumber = index & 0xFFFF`, `questGroup = (index & 0xFFFF0000) >> 16`
