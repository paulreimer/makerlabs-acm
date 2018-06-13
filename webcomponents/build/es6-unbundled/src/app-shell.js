define(["../node_modules/@polymer/polymer/polymer-element.js","../node_modules/@polymer/app-layout/app-header-layout/app-header-layout.js","../node_modules/@polymer/app-layout/app-header/app-header.js","../node_modules/@polymer/app-layout/app-toolbar/app-toolbar.js","./user-search-bar.js","./view-user-form.js","../node_modules/@polymer/polymer/lib/utils/html-tag.js"],function(_polymerElement,_appHeaderLayout,_appHeader,_appToolbar,_userSearchBar,_viewUserForm,_htmlTag){"use strict";class AppShell extends _polymerElement.PolymerElement{static get template(){return _htmlTag.html`
    <style>
      app-toolbar {
        background-color: #1E88E5;
        font-family: 'Roboto', Helvetica, sans-serif;
        color: white;
        --app-toolbar-font-size: 24px;
      }
      .search-bar {
        width: 50%;
      }
      .search-bar vaadin-combo-box {
        width: 50%;
      }
    </style>

    <app-header-layout>
      <app-header slot="header" fixed="">
        <app-toolbar>
          <google-signin client-id="837020778796-jav3n4g1fdse2f6s2qrbvm5n1koc93ci.apps.googleusercontent.com" scopes="https://www.googleapis.com/auth/spreadsheets https://www.googleapis.com/auth/drive"></google-signin>
          <div main-title="">
            <span>MakerLabs ACM</span>
          </div>
          <user-search-bar class="search-bar"></user-search-bar>
        </app-toolbar>
      </app-header>
      <view-user-form fields="[[fields]]" query="[[query]]">
        <google-client-loader id="sheets" name="sheets" version="v4"></google-client-loader>
      </view-user-form>
    </app-header-layout>
`}static get is(){return"app-shell"}static get properties(){return{fields:{type:Array},userName:{type:String},query:{type:Object}}}}customElements.define(AppShell.is,AppShell)});