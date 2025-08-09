# Run Configurations for Vite Project

This document describes the run configurations that have been set up for this Vite project.

## Available Run Configurations

The following run configurations have been created in the `.run` directory:

1. **dev** - Starts the Vite development server
   - Command: `npm run dev`
   - Use this during development to run the application with hot module replacement

2. **build** - Builds the project for production
   - Command: `npm run build`
   - Use this to create a production-ready build of the application

3. **preview** - Previews the production build
   - Command: `npm run preview`
   - Use this to preview the production build locally before deployment

4. **lint** - Runs ESLint to check code quality
   - Command: `npm run lint`
   - Use this to check for code style issues and potential bugs

## How to Use

### In JetBrains IDEs (WebStorm, IntelliJ IDEA, etc.)

1. Open the project in your JetBrains IDE
2. The run configurations will be automatically loaded
3. Click on the run configuration dropdown in the top-right corner of the IDE
4. Select the desired run configuration
5. Click the run button (green triangle) to execute the selected configuration

### Manually

If you prefer to run the commands manually, you can use the following npm commands in the terminal:

```bash
# Start development server
npm run dev

# Build for production
npm run build

# Preview production build
npm run preview

# Run linting
npm run lint
```

## Configuration Details

The Vite development server is configured with a proxy that redirects `/api` requests to `http://localhost:18080`. This allows the frontend to communicate with the backend API without CORS issues during development.

## Troubleshooting

If you encounter any issues with the run configurations:

1. Make sure Node.js is installed and properly configured in your IDE
2. Verify that all dependencies are installed by running `npm install`
3. Check the console output for any error messages
4. Ensure that the backend server is running when using features that require API access