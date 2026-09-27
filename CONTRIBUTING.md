# Contributing

Contributions are welcome, including bug reports, fixes, documentation updates,
and examples for additional boards or displays.

## Before You Start

- Search existing issues and pull requests to avoid duplicate work.
- For a substantial change, open an issue first to discuss the approach.
- Keep changes focused and preserve the generic, callback-based core.
- Do not include proprietary vendor files, generated build output, or unrelated
  project files.

## Changes to the Driver

- Keep hardware-specific code in `ports/` or `examples/`; keep the core driver
  independent of any MCU SDK.
- Preserve existing public APIs where practical. Document API changes in
  `docs/API.md` and user-facing behavior in `README.md` or the relevant guide.
- Follow the repository's C formatting and naming conventions. Keep source
  lines at or below 80 characters.
- Add or update examples when a change affects platform integration.
- Update `CHANGELOG.md` for user-visible changes.

## Validation

Before opening a pull request, compile the core driver with warnings enabled:

```sh
gcc -std=c99 -Wall -Wextra -Werror -c ST77916.c -o ST77916.o
```

Also check that modified C and header files do not exceed 80 characters per
line. If you change an adapter or example, validate it with its target SDK when
available and include the target board and toolchain details in the pull request.

## Pull Requests

- Use the pull request template and describe the user-visible change.
- Link related issues and call out API or compatibility changes.
- Include the validation performed and its result. State clearly when hardware
  testing was not possible.
- Keep each pull request focused; split unrelated changes where practical.

By submitting a contribution, you agree that it may be distributed under the
MIT License used by this project.
