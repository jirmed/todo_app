# Verzování Docker obrazů

## Implementované řešení

V projektu bylo implementováno verzování Docker obrazů dvěma způsoby:

1. **Git SHA verzování** - každý obraz je označen zkrácenou verzí Git SHA commitu
2. **Sémantické verzování** - při vytvoření Git tagu ve formátu `v*.*.*` se automaticky vytvoří Docker obrazy s odpovídajícími tagy

## Jak to funguje

### Tagy Docker obrazů

Při buildu se vytvářejí následující tagy:

- `latest` - pro hlavní větev (main)
- `sha-XXXXXXX` - krátká verze Git SHA pro každý build
- `v1.2.3` - přesná verze podle Git tagu (pokud existuje)
- `v1.2` - major.minor verze podle Git tagu

### Metadata v obrazech

Každý obraz obsahuje:

- OCI Labels s informacemi o verzi a Git SHA
- Soubor `/app/version.txt` s informacemi o verzi pro debugování

## Jak vytvořit novou verzi

1. Vytvořte Git tag ve formátu sémantického verzování:
   ```
   git tag -a v1.0.0 -m "Vydání verze 1.0.0"
   git push origin v1.0.0
   ```

2. CI/CD pipeline automaticky:
   - Vytvoří Docker obrazy s příslušnými tagy
   - Nasadí aplikaci do produkce s novými verzemi obrazů

## Nasazení specifické verze

V souboru docker-compose.yaml můžete specifikovat konkrétní verzi místo `latest` tagu:

```yaml
services:
  backend:
    image: ghcr.io/jirmed/todo-backend:v1.0.0
```

## Kontrola verzí

Pro zjištění verze běžícího kontejneru můžete použít:

```bash
docker exec <container_id> cat /app/version.txt
```

Nebo si prohlédnout historii nasazení v souboru `/home/ubuntu/deployment_history.log` na produkčním serveru.
