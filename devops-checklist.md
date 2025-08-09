# Akční checklist po DevOps auditu

## 1. Prioritizovaný checklist úkolů

### Krátkodobé úkoly (do 1 týdne)

| Úkol | Dopad | Obtížnost |
|------|-------|------------|
| 🔄 Implementovat verzování Docker obrazů (Git SHA nebo sémantické) | Vysoký | Nízká |
| 🩺 Přidat healthchecky do Docker Compose konfigurace | Střední | Nízká |
| 🔒 Zabezpečit Traefik (odstranit api.insecure=true, přidat TLS) | Vysoký | Střední |
| 🧪 Aktivovat testování v CI pipeline pro backend | Vysoký | Střední |
| 📊 Implementovat základní monitoring (Prometheus + Grafana) | Střední | Střední |
| 🔍 Přidat logování pro backend aplikaci | Střední | Nízká |

### Dlouhodobé úkoly (1-3 měsíce)

| Úkol | Dopad | Obtížnost |
|------|-------|------------|
| 🔄 Vytvořit staging prostředí s automatickým deploymentem | Vysoký | Střední |
| 🔄 Implementovat blue-green deployment nebo canary releases | Vysoký | Vysoká |
| 🔒 Nastavit automatizované testování bezpečnosti (SAST, container scanning) | Vysoký | Střední |
| 📊 Vytvořit robustní logging stack (ELK/EFK) | Střední | Vysoká |
| 🏗️ Implementovat infrastructure-as-code (Terraform/Pulumi) | Vysoký | Vysoká |
| 🔄 Nastavit automatickou rotaci přístupových klíčů | Střední | Střední |

## 2. Závislosti

- Testování v CI pipeline **musí být hotové před** implementací blue-green deploymentu
- Staging prostředí **musí být hotové před** implementací canary releases
- Základní monitoring **musí být hotový před** implementací robustního logging stacku
- Verzování Docker obrazů **musí být hotové před** nastavením blue-green deploymentu

## 3. Doporučené nástroje a zdroje

- **Docker Healthchecks**: https://docs.docker.com/compose/compose-file/compose-file-v3/#healthcheck
- **Traefik TLS konfigurace**: https://doc.traefik.io/traefik/https/tls/
- **GitHub Actions matrix**: https://docs.github.com/en/actions/using-jobs/using-a-matrix-for-your-jobs
- **Prometheus + Grafana**: https://prometheus.io/docs/visualization/grafana/
- **Docker multi-stage build optimalizace**: https://docs.docker.com/build/building/multi-stage/
- **Blue-Green Deployment**: https://martinfowler.com/bliki/BlueGreenDeployment.html

## 4. Quick wins

1. **Verzování Docker obrazů** - Jednoduše upravit GitHub Actions workflow pro generování tagů z Git SHA nebo verze
   ```yaml
   - name: Set image tag
     id: vars
     run: echo "tag=$(echo ${GITHUB_SHA} | cut -c1-7)" >> $GITHUB_OUTPUT
   ```
   a poté použít `ghcr.io/jirmed/todo-backend:${{ steps.vars.outputs.tag }}` místo `latest`

2. **Healthchecky v Docker Compose** - Přidat healthcheck sekci ke každé službě:
   ```yaml
   healthcheck:
     test: ["CMD", "curl", "-f", "http://localhost:18080/api/health"]
     interval: 30s
     timeout: 10s
     retries: 3
     start_period: 40s
   ```

3. **Logování backendu** - Přidat konfiguraci logování do C++ aplikace a zajistit výstup do stdout/stderr pro integraci s Docker logging driverem

4. **Odstranit insecure Traefik API** - Změnit konfiguraci Traefik v docker-compose.yaml:
   ```yaml
   command:
     - "--api.insecure=false"
     - "--api.dashboard=true"
     - "--providers.docker=true"
   ```

5. **Nastavit resource limits** pro kontejnery v docker-compose.yaml:
   ```yaml
   deploy:
     resources:
       limits:
         cpus: '0.5'
         memory: 512M
   ```
