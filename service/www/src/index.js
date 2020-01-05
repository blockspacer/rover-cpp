// import 'react-app-polyfill/ie11'; // For IE 11 support
import 'react-app-polyfill/stable'; // For other old browsers support
import React from 'react';
import ReactDOM from 'react-dom';
import './index.scss';
import App from './App';
import * as serviceWorker from './serviceWorker';
// import 'react-app-polyfill/jsdom';

ReactDOM.render(<App/>, document.getElementById('root'));

// If you want your app to work offline and load faster, you can change
// unregister() to register() below. Note this comes with some pitfalls.
// Learn more about service workers: https://bit.ly/CRA-PWA
serviceWorker.unregister();
