import { initializeApp } from "firebase/app";
import { getDatabase } from 'firebase/database';

const config = {
    apiKey: "AIzaSyBI0952DSKWRpR76ITAvnKcdZvSNRtQx7Q",
    authDomain: "solarpaneldb.firebaseapp.com",
    databaseURL: "https://solarpaneldb-default-rtdb.firebaseio.com",
    projectId: "solarpaneldb",
    storageBucket: "solarpaneldb.appspot.com",
    messagingSenderId: "952981786064",
    appId: "1:952981786064:web:5c7a76252ccd1ca9337dd5",
    measurementId: "G-6KHSPK8HTZ"
};

const app = initializeApp(config)
const database = getDatabase(app);

export default database