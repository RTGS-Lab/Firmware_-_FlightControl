# FlightControl Demo GitHub Actions Workflows

This folder contains GitHub Actions workflows for automating the testing, building, and deployment of the FlightControl Demo firmware.

## Workflow Overview

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

## Manual Workflow Triggering

Both workflows can be triggered manually from the GitHub Actions tab:

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

1. These workflows run automatically based on PR and push events to the main branch.
2. The release workflow will only create a new GitHub release and upload to Particle if the auto-version step updates the firmware version.
3. The test pipeline ensures that only code that passes tests can be merged.