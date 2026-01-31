# Security Policy

## Supported Versions

| Version | Supported          |
| ------- | ------------------ |
| 0.1.x   | :white_check_mark: |
| < 0.1.0 | :x:                |

## Reporting a Vulnerability

If you discover a security vulnerability in DIPAL, please report it responsibly.

### How to Report

1. **Do NOT open a public issue** for security vulnerabilities
2. Email the maintainer directly at: **paulkokos@yahoo.gr**
3. Include the following in your report:
   - Description of the vulnerability
   - Steps to reproduce
   - Potential impact
   - Suggested fix (if any)

### What to Expect

- **Acknowledgment**: Within 48 hours of your report
- **Initial Assessment**: Within 7 days
- **Resolution Timeline**: Depends on severity
  - Critical: 7 days
  - High: 14 days
  - Medium: 30 days
  - Low: 60 days

### Disclosure Policy

- We follow coordinated disclosure
- We will credit reporters in the security advisory (unless you prefer to remain anonymous)
- Please allow us reasonable time to address the issue before public disclosure

## Security Considerations

DIPAL is an image processing library. When using it, be aware of:

### Input Validation
- Always validate image files from untrusted sources
- Large or malformed images may cause high memory usage
- The library attempts to handle malformed input gracefully via Result types

### Memory Safety
- DIPAL uses modern C++23 with bounds checking where possible
- Image data is managed through RAII patterns
- No raw pointer arithmetic in public APIs

### Dependencies
- Minimal external dependencies
- Dependencies are fetched via CMake FetchContent from trusted sources

## Security Updates

Security updates will be released as patch versions (e.g., 0.1.x) and announced via:
- GitHub Security Advisories
- Release notes

## Contact

Maintainer: Pavlos Kokozidis
Email: paulkokos@yahoo.gr
GitHub: [@paulkokos](https://github.com/paulkokos)
