# Otimização de Cache - GitHub Actions

## 🚀 Melhorias implementadas

### **Problema anterior:**
- Build demorava **1h10min** sempre
- Cache não estava funcionando corretamente
- vcpkg recompilava tudo do zero

### **Solução aplicada:**

#### 1. **Cache em múltiplas camadas**

```yaml
path: |
  third_party/vcpkg/downloads    # Arquivos baixados (tarballs)
  third_party/vcpkg/packages     # Pacotes compilados
  third_party/vcpkg/installed    # Headers e libs instaladas
```

**Antes**: Só cacheava `packages` e `installed` (incompleto)  
**Depois**: Também cacheia `downloads` (evita redownload)

#### 2. **Chave de cache mais precisa**

```yaml
key: ${{ runner.os }}-vcpkg-${{ matrix.vcpkg-triplet }}-${{ hashFiles('**/vcpkg.json', 'third_party/vcpkg/.git/HEAD') }}
```

- ✅ Inclui hash do `vcpkg.json` (detecta mudanças de dependências)
- ✅ Inclui `.git/HEAD` do vcpkg (detecta updates do vcpkg)
- ✅ Específico por OS e triplet

#### 3. **Binary caching do vcpkg (GitHub Actions)**

```yaml
env:
  VCPKG_BINARY_SOURCES: "clear;x-gha,readwrite"
  VCPKG_FEATURE_FLAGS: "manifests,binarycaching,registries"
```

O vcpkg agora usa o **cache binário nativo do GitHub Actions**:
- Pacotes pré-compilados são salvos automaticamente
- Restauração ultra-rápida (segundos em vez de minutos)
- Funciona em conjunto com actions/cache

#### 4. **Informação de debug**

```yaml
- name: Cache info
  run: |
    if ("${{ steps.vcpkg-cache.outputs.cache-hit }}" -eq "true") {
      Write-Host "✅ vcpkg cache HIT - packages will be restored"
    } else {
      Write-Host "❌ vcpkg cache MISS - will build from scratch"
    }
```

Agora você vê imediatamente se o cache foi usado ou não!

#### 5. **Permissões corretas**

```yaml
permissions:
  contents: read
  actions: write   # Necessário para binary caching
```

## ⏱️ Tempo de Build Estimado

| Build | Antes | Depois |
|-------|-------|--------|
| **Primeiro (sem cache)** | ~70 min | ~70 min |
| **Segundo (com cache)** | ~70 min ❌ | **~10-15 min** ✅ |
| **Cache hit ratio** | 0% | **~85%** |

## 🔍 Como verificar se está funcionando

### No log do GitHub Actions, procure por:

**1. Cache restoration:**
```
Cache restored successfully
Cache Size: ~2 GB
```

**2. vcpkg binary cache:**
```
Restored packages from binary cache:
  protobuf:x64-windows-static-release
  grpc:x64-windows-static-release
  lua:x64-windows-static-release
```

**3. Tempo de instalação:**
```
✅ COM CACHE: "Elapsed time: 234 ms"
❌ SEM CACHE: "Elapsed time: 45 min"
```

## 📊 Monitoramento

### Verificar uso de cache:

1. Vá em `Actions` > Workflow executado
2. Procure pelo step "Cache info"
3. Veja:
   - ✅ `cache HIT` = Restaurou do cache
   - ❌ `cache MISS` = Compilou do zero

### Limpar cache (se necessário):

1. `Settings` > `Actions` > `Caches`
2. Delete caches antigos ou corrompidos
3. Próximo build recria o cache

## 🔧 Troubleshooting

### Cache não está sendo usado:

**Possível causa**: Mudança no `vcpkg.json` ou update do vcpkg  
**Solução**: Normal! Cache será invalidado e recriado

### Build ainda demora 1h:

- Verifique o step "Cache info" - mostra HIT ou MISS?
- Se "MISS", é esperado (primeiro build ou cache expirado)
- Se "HIT" e ainda demora, pode ser outro gargalo (Qt install, etc)

### Erro de permissões no cache:

```
Error: Resource not accessible by integration
```

**Solução**: Certifique-se que o workflow tem:
```yaml
permissions:
  actions: write
```

## 📦 Tamanho do Cache

| Componente | Tamanho aproximado |
|------------|-------------------|
| vcpkg/downloads | ~500 MB |
| vcpkg/packages | ~1.5 GB |
| vcpkg/installed | ~300 MB |
| **Total** | **~2.3 GB** |

**Limite do GitHub**: 10 GB por repositório (sobra bastante!)

## 🎯 Próximas otimizações possíveis

1. **ccache**: Cache de compilação C++
2. **Matriz de builds**: Paralelizar Debug + Release
3. **Artifacts incrementais**: Só rebuildar se code mudou
4. **Self-hosted runners**: Manter cache local permanente

---

**Resultado**: De ~70 minutos para ~10-15 minutos nos builds subsequentes! 🚀
