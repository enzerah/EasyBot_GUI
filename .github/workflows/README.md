# GitHub Actions Workflows

## Build EasyBot_GUI

Este workflow compila automaticamente o EasyBot_GUI a cada push ou pull request.

### Melhorias implementadas:

✅ **Cache inteligente do vcpkg** - Reduz o tempo de build em ~50 minutos
✅ **Cache do Qt** - Evita download repetido do Qt
✅ **Busca automática do grpc_cpp_plugin** - Não depende de paths hardcoded
✅ **Qt PATH dinâmico** - Funciona tanto local quanto no CI
✅ **Workflow_dispatch** - Permite executar manualmente pela interface do GitHub
✅ **Artifacts organizados** - Só faz upload dos executáveis necessários

### Executando manualmente:

1. Vá em `Actions` no repositório
2. Selecione `Build EasyBot_GUI`
3. Clique em `Run workflow`

### Tempo estimado:

- **Primeira execução**: ~1 hora (instala tudo)
- **Builds subsequentes**: ~15-20 minutos (usa cache)

### Baixando os executáveis:

Após o build completar, vá em:
1. `Actions` > Workflow executado
2. Role até o final
3. Clique em `EasyBot_GUI-Release` para baixar
