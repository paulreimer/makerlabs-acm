import { PolymerElement } from '@polymer/polymer/polymer-element.js';

import { html } from '@polymer/polymer/lib/utils/html-tag.js';

class ImageFileUploader extends PolymerElement {
  static get template() {
    return html`
    <style>
      #drop_zone {
        height: 200px;
        border: 0px dashed #bbb;
        -moz-border-radius: 5px;
        -webkit-border-radius: 5px;
        border-radius: 5px;
        padding: 25px;
        margin-bottom: 8px;
        text-align: center;
        font: 20pt bold 'Helvetica';
        color: #bbb;
        background-size: contain;
        background-repeat: no-repeat;
        background-position: center;
      }
      #drop_hint {
        display: none;
      }
    </style>

    <div id="drop_zone" style="background-image: url([[src]]);">
      <span id="drop_hint">Drop files here</span>
    </div>
`;
  }

  static get is() { return 'image-file-uploader'; }

  static get properties() {
    return {
      src: {
        type: String,
        value: "data:image/gif;base64,R0lGODlhAQABAAD/ACwAAAAAAQABAAACADs="
      },
      emptyImageData: {
        type: String,
        value: "data:image/gif;base64,R0lGODlhAQABAAD/ACwAAAAAAQABAAACADs="
      }
    }
  }

  get accessToken() {
    var accessToken = null;

    var authInstance = (
      gapi
      && gapi.auth2
      && gapi.auth2.getAuthInstance()
    );

    if (authInstance)
    {
      var currentUser = (
        authInstance
        && authInstance.currentUser
        && authInstance.currentUser.get()
      );

      if (currentUser) {
        var authResponse = currentUser.getAuthResponse(true);
        if (authResponse) {
          if ('access_token' in authResponse) {
            accessToken = authResponse['access_token'];
          }
        }
      }
    }

    return accessToken;
  }

  // Called when files are dropped on to the drop target. For each file,
  // uploads the content to Drive & displays the results when complete.
  handleFileSelect(evt) {
    evt.stopPropagation();
    evt.preventDefault();

    if (this.accessToken)
    {
      var files = evt.dataTransfer.files; // FileList object.

      var output = [];
      for (var i = 0, f; f = files[i]; i++) {
        var uploader = new MediaUploader({
          file: f,
          token: this.accessToken,
          onComplete: function(json) {
            this.handleDragLeave();

            // Parse the uploaded file response metadata
            var data = JSON.parse(json);

            // Check if a valid web content link was created/found
            if (data && data['webContentLink']) {
              // Update displayed image
              this.src = data['webContentLink'];
            }
          }.bind(this)
        });
        uploader.upload();
      }
    }
    else {
      console.log("Missing accessToken");
    }
  }

  // Dragover handler to set the drop effect.
  handleDragOver(evt) {
    var el = this.shadowRoot.getElementById('drop_zone');
    if (el) {
      el.style.borderWidth = '2px';
    }

    var hint = this.shadowRoot.getElementById('drop_hint');
    if (hint) {
      hint.style.display = 'block';
    }

    if (evt) {
      evt.stopPropagation();
      evt.preventDefault();
      evt.dataTransfer.dropEffect = 'copy';
    }
  }

  // Dragover handler to set the drop effect.
  handleDragLeave(evt) {
    var el = this.shadowRoot.getElementById('drop_zone');
    if (el) {
      el.style.borderWidth = '0px';
    }

    var hint = this.shadowRoot.getElementById('drop_hint');
    if (hint) {
      hint.style.display = 'none';
    }

    if (evt) {
      evt.stopPropagation();
      evt.preventDefault();
    }
  }

  ready() {
    super.ready();

    // Wire up drag & drop listeners once page loads
    var el = this.shadowRoot.getElementById('drop_zone');
    el.addEventListener('dragover', this.handleDragOver.bind(this), false);
    el.addEventListener('dragleave', this.handleDragLeave.bind(this), false);
    el.addEventListener('drop', this.handleFileSelect.bind(this), false);
  }
}

customElements.define(ImageFileUploader.is, ImageFileUploader);
