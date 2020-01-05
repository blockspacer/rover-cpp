import React from 'react';
import 'react-dates/initialize';
import './fontAwesome';
import './App.scss';
import {Container} from "reactstrap";
import {Navbar, NavDropdown} from "react-bootstrap";
import {Row, Col} from "react-bootstrap";

import Clocks from "components/Clocks";
import Monitoring from "components/Monitoring";
import config from "config/config";


const App = () => {
    return (
        <Container fluid>
            <Navbar bg="dark" variant="dark">
                <Navbar.Brand href="#home">
                    <img
                        alt=""
                        src="menu-green-button.png"
                        width="32"
                        height="32"
                        className="d-inline-block align-top"
                    />
                    {' Rover'}
                </Navbar.Brand>
                <Navbar.Toggle aria-controls="responsive-navbar-nav"/>
                /*
                <Navbar.Collapse id="responsive-navbar-nav">
                    <NavDropdown title="Menu" id="collasible-nav-dropdown">
                        <NavDropdown.Item href="#monitoring">Monitoring</NavDropdown.Item>
                    </NavDropdown>
                </Navbar.Collapse>
                */
                <Clocks/>
            </Navbar>
            <Row>
                <Col>{process.env.NODE_ENV}</Col>
                <Col md={{offset: 8}}><Monitoring title="System info" config={config}/></Col>
            </Row>
        </Container>
    );
};

export default App;
