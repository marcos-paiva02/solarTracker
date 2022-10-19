import React, { useState, useEffect } from 'react'
import Measures from './Measures'
import Measures2 from './Measures2'
import Statistic from './Statistic'
import Statistic2 from './Statistic2'
import firebase from './firebase'
import { ref, onValue, set} from 'firebase/database'

import '../css/landing.css'

import logo from '../img/logo.svg'
import insta from  '../img/insta.png'
  
export default () => {
    
    const [status, setStatus] = useState([])
    const statusRef = ref(firebase, ('status'))

    function updateStatus (val) {
        set(ref(firebase, 'status/'), {
            status: val,
            bateria: status.bateria
        })
    }

    useEffect(() => {
        onValue(statusRef, (snapshot) => {
            setStatus(snapshot.val())
        })
    }, [])

    return (
        <div className="page-landing" id="page-landing">
            <header className="landing animate-up">
                <aside>
                    <a href="https://www.instagram.com/dudink.tattoo/" target="_blank">
                        <img src={insta} alt=""/>
                    </a>
                </aside>
                <div className="header-container">
                    <div className="logo-titulo">
                        <img className="logo" src={logo} alt="logo"/>
                        <strong className="titulo">RASTREADOR SOLAR SMART</strong>
                    </div>
                    <div className='status-container'>
                        <div className= {`status-off ${status.status === 'LIGADO' ? 'status-on' : '' }`}> 
                            STATUS RASTREADOR: { (status.status) }
                            <button className='status-button' onClick={() => {
                                if (status.status === 'LIGADO') {
                                    updateStatus('DESLIGADO')
                                } else {
                                    updateStatus('LIGADO')
                                }
                            }}>ON / OFF</button>
                        </div>
                        <div className='bateria'> NÍVEL DA BATERIA: { (status.bateria) }% </div>
                    </div>
                    <div className='datas-container'>
                        <div className='tasks-container'>
                            <div className='cart'> Banho de 20 minutos </div>
                            <div className='cart'> Passar roupa por 1 hora </div>
                            <div className='cart'> Aquecer Piscina </div>
                            <div className='cart'></div>
                            <div className='cart'></div>
                            <div className='cart'></div>
                            <div className='cart'></div>
                        </div>
                        <div className="components-label">
                            <div><Statistic /></div>
                            <div><Measures /></div>
                            <div><Statistic2 /></div>
                            <div><Measures2 /></div>
                        </div>
                    </div>
                </div>
            </header>
        </div>
    )
}