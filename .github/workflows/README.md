# FlightControl Demo GitHub Actions Workflows

This folder contains GitHub Actions workflows for automating the testing, building, and deployment of the FlightControl Demo firmware.

## Automated Workflow Overview

### PR Workflow (`pr-workflow.yaml`)

Triggered in the following scenarios:
- When a pull request is opened, reopened, or updated (synchronize) against the `master` branch
- Manually from the GitHub Actions tab

This workflow has three sequential jobs:

1. **Test**: Runs unit tests using CMake
2. **Compile**: Compiles the firmware for the BSOM platform 
3. **Flash**: Flashes the compiled firmware to a test device

Each job only runs if the previous job succeeds, providing a complete validation pipeline for pull requests.

### Release Workflow (`release-workflow.yaml`)

Triggered in the following scenarios:
- When a PR is merged to `master` (note: closing without merging won't trigger the release)
- When code is pushed directly to `master`
- Manually from the GitHub Actions tab

This workflow has two sequential jobs:

1. **Release**: 
   - Compiles the firmware with auto-versioning
   - Commits the updated version file
   - Creates a GitHub release with the firmware binary and debug information

2. **Upload**:
   - Uploads the firmware to the Particle cloud for OTA updates to production devices
   - Only runs if the firmware version was updated in the previous step

## Individual Manual Workflows

These workflows can only be triggered manually from the GitHub Actions tab and are designed for testing individual steps before opening a PR.

### Manual Test (`manual-test.yaml`)
Runs the unit tests on a specific branch.
- **Inputs**:
  - Branch to test (optional)

### Manual Compile (`manual-compile.yaml`)
Compiles the firmware on a specific branch.
- **Inputs**:
  - Branch to compile (optional)
- **Outputs**:
  - Firmware binary as an artifact

### Manual Flash (`manual-flash.yaml`)
Flashes a device with compiled firmware.
- **Inputs**:
  - Device ID (optional, defaults to test device)
  - Use latest artifact (yes/no)
- **Notes**: 
  - Can use the firmware from the latest manual-compile run or compile a new binary

### Manual Release (`manual-release.yaml`)
Creates a test release without committing version changes.
- **Inputs**:
  - Branch to release (optional)
  - Force version increment (yes/no)
  - Custom release notes (optional)
- **Notes**: 
  - Creates a GitHub prerelease with '-manual' tag suffix
  - Doesn't commit version changes back to the repository

### Manual Upload (`manual-upload.yaml`)
Uploads a firmware binary to Particle cloud.
- **Inputs**:
  - Product ID (optional)
  - Firmware version (required)
  - Release title (optional)
  - Use release artifact (yes/no)
- **Notes**:
  - Can use the firmware from the latest manual-release run or compile a new binary

## Manual Workflow Triggering (Main Workflows)

The main workflows can also be triggered manually from the GitHub Actions tab:

1. **PR Workflow**:
   - Optional input for the reason for manual testing

2. **Release Workflow**:
   - Option to control version increment (yes/no)
   - Custom release notes field that overrides the auto-generated ones

## Required Secrets

The workflows require the following secrets to be configured in your GitHub repository:

- `PARTICLE_ACCESS_TOKEN`: Particle cloud API token for uploading firmware
- `PARTICLE_USER_LEVEL_ACCESS_TOKEN`: Particle token for flashing individual test devices
- `PARTICLE_TEST_DEVICE_ID`: ID of the test device to flash during PR testing
- `PARTICLE_PRODUCT_ID`: ID of the Particle product for production releases

## Event Flow

1. **When opening a PR to master**:
   - PR Workflow runs (test → compile → flash)
   - This validates the changes before they're merged

2. **When merging a PR to master**:
   - Release Workflow runs (compile/release → upload)
   - This creates a new version, GitHub release, and deploys to Particle

3. **When pushing directly to master**:
   - Release Workflow runs (same as merging a PR)
   - This is for quick changes that don't require a PR

## Usage Notes

1. For initial testing, use the individual manual workflows to check each step separately
2. Once confident, open a PR to trigger the full PR workflow
3. After reviewing and approving, merge the PR to trigger the release workflow
4. Manual workflows provide a way to test specific stages of the pipeline in isolation