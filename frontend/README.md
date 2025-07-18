# React + TypeScript + Vite

This template provides a minimal setup to get React working in Vite with HMR and some ESLint rules.

## API Configuration

### Development Environment

In the development environment, the backend API runs on port 18080. The application is configured to handle API requests through the Vite development server proxy:

1. **Vite Proxy**: The Vite development server is configured with a proxy that redirects `/api` requests to `http://localhost:18080`. This is set up in `vite.config.ts` with enhanced debugging capabilities:

```javascript
// vite.config.ts
export default defineConfig({
  plugins: [react()],
  server: {
    proxy: {
      '/api': {
        target: 'http://localhost:18080',
        changeOrigin: true,
        secure: false,
        rewrite: (path) => path,
        configure: (proxy, _options) => {
          proxy.on('error', (err, _req, _res) => {
            console.log('proxy error', err);
          });
          proxy.on('proxyReq', (proxyReq, req, _res) => {
            console.log('Sending Request to the Target:', req.method, req.url);
          });
          proxy.on('proxyRes', (proxyRes, req, _res) => {
            console.log('Received Response from the Target:', proxyRes.statusCode, req.url);
          });
        },
      },
    },
  },
})
```

The proxy configuration includes:
- `target`: The backend server URL
- `changeOrigin`: Changes the origin of the request to match the target URL
- `secure`: Allows proxying to servers with invalid SSL certificates
- `rewrite`: Keeps the path as is when forwarding the request
- `configure`: Sets up event listeners for debugging proxy issues

2. **API URL Helper**: The application includes a helper function that uses relative URLs for all API endpoints:

```typescript
const getApiUrl = (endpoint: string): string => {
    // Používáme relativní cestu pro všechny požadavky, aby šly přes proxy
    return endpoint;
};
```

This function is used for all API requests in the application:

```typescript
// For all requests (GET, POST, PATCH, DELETE)
fetch(getApiUrl('/api/tasks'))
fetch(getApiUrl(`/api/tasks/${id}`), { method: 'PATCH', ... })
```

Using relative URLs for all requests ensures they go through the Vite proxy in development mode, which prevents CORS issues by handling the cross-origin requests on the server side.

### Production Environment

In the production environment (Docker), the API runs on the backend at `/api` on the same port as the frontend through a Traefik reverse proxy. The application uses relative URLs for API requests, which are correctly routed by the reverse proxy.

## Vite Plugins

Currently, two official plugins are available:

- [@vitejs/plugin-react](https://github.com/vitejs/vite-plugin-react/blob/main/packages/plugin-react) uses [Babel](https://babeljs.io/) for Fast Refresh
- [@vitejs/plugin-react-swc](https://github.com/vitejs/vite-plugin-react/blob/main/packages/plugin-react-swc) uses [SWC](https://swc.rs/) for Fast Refresh

## Expanding the ESLint configuration

If you are developing a production application, we recommend updating the configuration to enable type-aware lint rules:

```js
export default tseslint.config([
  globalIgnores(['dist']),
  {
    files: ['**/*.{ts,tsx}'],
    extends: [
      // Other configs...

      // Remove tseslint.configs.recommended and replace with this
      ...tseslint.configs.recommendedTypeChecked,
      // Alternatively, use this for stricter rules
      ...tseslint.configs.strictTypeChecked,
      // Optionally, add this for stylistic rules
      ...tseslint.configs.stylisticTypeChecked,

      // Other configs...
    ],
    languageOptions: {
      parserOptions: {
        project: ['./tsconfig.node.json', './tsconfig.app.json'],
        tsconfigRootDir: import.meta.dirname,
      },
      // other options...
    },
  },
])
```

You can also install [eslint-plugin-react-x](https://github.com/Rel1cx/eslint-react/tree/main/packages/plugins/eslint-plugin-react-x) and [eslint-plugin-react-dom](https://github.com/Rel1cx/eslint-react/tree/main/packages/plugins/eslint-plugin-react-dom) for React-specific lint rules:

```js
// eslint.config.js
import reactX from 'eslint-plugin-react-x'
import reactDom from 'eslint-plugin-react-dom'

export default tseslint.config([
  globalIgnores(['dist']),
  {
    files: ['**/*.{ts,tsx}'],
    extends: [
      // Other configs...
      // Enable lint rules for React
      reactX.configs['recommended-typescript'],
      // Enable lint rules for React DOM
      reactDom.configs.recommended,
    ],
    languageOptions: {
      parserOptions: {
        project: ['./tsconfig.node.json', './tsconfig.app.json'],
        tsconfigRootDir: import.meta.dirname,
      },
      // other options...
    },
  },
])
```
