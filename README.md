# GDIntercept Plus

A small Geode add-on for [GDIntercept](https://github.com/SMJSProductions/GDIntercept). It adds a **+** button
to GDIntercept's capture window that opens a **New Request** window, where you can draft a request and send it.

This is a separate mod. It contains no GDIntercept code (GDIntercept is all rights reserved); it only adds a
button to its window.

## Using it

- Open GDIntercept (default Alt+I) and click the **+** next to its settings gear, or press **Alt+P** anywhere.
- Pick a method, type a URL, add headers and a body, and press **Send**.
- Requests sent this way use Geode's web client, so GDIntercept lists them like any other request.

### Fields

- **Headers**: `Name: value` entries separated by `\n` (a backslash and an n), for example
  `Content-Type: application/json\nX-Test: 1`.
- **Body**: sent as-is for methods that take one. If you don't set a Content-Type, it defaults to
  `application/x-www-form-urlencoded`.
- A bare host such as `example.com/x` becomes `https://example.com/x`.

### Buttons

- **Quick link**: cycles through a few ready-made URLs (a safe echo service, the GD server base URL, the Geode API).
  Only URLs are filled in. Nothing reads your login or any secret keys.
- **Paste curl**: fills the form from a curl command on your clipboard, such as the one GDIntercept's
  *Copy Curl* produces. A plain link works too.
- **Copy**: copies the full response. **Clear**: empties the form.

Your last draft is remembered between sessions.

## Building

Push to GitHub; the included workflow builds the `.geode` file (see the Actions tab).
